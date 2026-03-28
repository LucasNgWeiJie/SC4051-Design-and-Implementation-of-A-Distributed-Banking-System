package api

import (
	"bank_server/biz"
	"bank_server/common"
	"bank_server/data"
	"bytes"
	"encoding/binary"
	"fmt"
	"net"
	"sync"
	"time"
)

type Router struct {
	openAccSvc  *biz.OpenAccountService
	closeAccSvc *biz.CloseAccountService
	depWdSvc    *biz.DepositWithdrawService
	chkBalSvc   *biz.CheckBalanceService
	transfSvc   *biz.TransferService
	monitorSvc  *biz.MonitorUpdatesService

	cache      map[string]map[uint32][]byte
	cacheMu    sync.Mutex
	monitors   map[string]time.Time
	monitorsMu sync.Mutex
}

func NewRouter(repo *data.AccountRepository) *Router {
	return &Router{
		openAccSvc:  biz.NewOpenAccountService(repo),
		closeAccSvc: biz.NewCloseAccountService(repo),
		depWdSvc:    biz.NewDepositWithdrawService(repo),
		chkBalSvc:   biz.NewCheckBalanceService(repo),
		transfSvc:   biz.NewTransferService(repo),
		monitorSvc:  biz.NewMonitorUpdatesService(repo),
		cache:       make(map[string]map[uint32][]byte),
		monitors:    make(map[string]time.Time),
	}
}

func (r *Router) RegisterMonitor(clientAddr string, interval int32) {
	r.monitorsMu.Lock()
	r.monitors[clientAddr] = time.Now().Add(time.Duration(interval) * time.Second)
	r.monitorsMu.Unlock()
}

func (r *Router) BroadcastUpdate(msg string, conn *net.UDPConn) {
	r.monitorsMu.Lock()
	defer r.monitorsMu.Unlock()

	now := time.Now()

	buf := new(bytes.Buffer)
	binary.Write(buf, binary.BigEndian, uint16(len(msg)))
	buf.WriteString(msg)
	msgBytes := buf.Bytes()

	headerBytes := make([]byte, 10)
	binary.BigEndian.PutUint32(headerBytes[0:4], 0)
	headerBytes[4] = common.OpCallbackUpdate
	headerBytes[5] = common.SemAtLeastOnce
	binary.BigEndian.PutUint32(headerBytes[6:10], uint32(len(msgBytes)))

	fullUpdate := append(headerBytes, msgBytes...)

	for addrStr, expiry := range r.monitors {
		if now.After(expiry) {
			delete(r.monitors, addrStr)
			continue
		}
		addr, err := net.ResolveUDPAddr("udp", addrStr)
		if err == nil {
			conn.WriteToUDP(fullUpdate, addr)
		}
	}
}

func (r *Router) HandleRequest(conn *net.UDPConn, clientAddr *net.UDPAddr, reqData []byte) {
	// Filter malformed packets
	if len(reqData) < 10 {
		return
	}

	// Parse custom binary header
	header := common.MessageHeader{}
	header.RequestID = binary.BigEndian.Uint32(reqData[0:4])
	header.Opcode = reqData[4]
	header.Semantics = reqData[5]
	header.PayloadLength = binary.BigEndian.Uint32(reqData[6:10])

	clientStr := clientAddr.String()

	if header.Semantics == common.SemAtMostOnce {
		r.cacheMu.Lock()
		if clientCache, ok := r.cache[clientStr]; ok {
			if replyData, exists := clientCache[header.RequestID]; exists {
				r.cacheMu.Unlock()
				fmt.Printf("Duplicate request %d from %s\n", header.RequestID, clientStr)
				conn.WriteToUDP(replyData, clientAddr)
				return
			}
		}
		r.cacheMu.Unlock()
	}

	// Extract payload and route to appropriate service
	payload := reqData[10:]
	var replyPayload []byte
	var isError bool

	switch header.Opcode {
	case common.OpOpenAccount:
		replyPayload, isError = r.openAccSvc.Execute(payload)
	case common.OpCloseAccount:
		replyPayload, isError = r.closeAccSvc.Execute(payload)
	case common.OpDepositWithdraw:
		replyPayload, isError = r.depWdSvc.Execute(payload)
	case common.OpCheckBalance:
		replyPayload, isError = r.chkBalSvc.Execute(payload)
	case common.OpTransfer:
		replyPayload, isError = r.transfSvc.Execute(payload)
	case common.OpMonitorUpdates:
		replyPayload, isError = r.monitorSvc.Execute(payload, clientStr, r.RegisterMonitor)
	default:
		// simple error payload creation duplicated here for unknown handler
		buf := new(bytes.Buffer)
		binary.Write(buf, binary.BigEndian, uint16(14))
		buf.WriteString("Unknown Opcode")
		replyPayload = buf.Bytes()
		isError = true
	}

	replyOpcode := common.OpReply
	if isError {
		replyOpcode = common.OpError
	}

	replyHeaderBytes := make([]byte, 10)
	binary.BigEndian.PutUint32(replyHeaderBytes[0:4], header.RequestID)
	replyHeaderBytes[4] = replyOpcode
	replyHeaderBytes[5] = header.Semantics
	binary.BigEndian.PutUint32(replyHeaderBytes[6:10], uint32(len(replyPayload)))
	fullReply := append(replyHeaderBytes, replyPayload...)

	if header.Semantics == common.SemAtMostOnce {
		r.cacheMu.Lock()
		if _, ok := r.cache[clientStr]; !ok {
			r.cache[clientStr] = make(map[uint32][]byte)
		}
		r.cache[clientStr][header.RequestID] = fullReply
		r.cacheMu.Unlock()
	}

	conn.WriteToUDP(fullReply, clientAddr)

	if !isError && header.Opcode != common.OpCheckBalance && header.Opcode != common.OpMonitorUpdates {
		r.BroadcastUpdate(fmt.Sprintf("Account update occurred. Opcode: %d", header.Opcode), conn)
	}
}
