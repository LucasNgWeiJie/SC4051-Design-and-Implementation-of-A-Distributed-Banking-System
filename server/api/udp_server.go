package api

import (
	"fmt"
	"math/rand"
	"net"
)

type UDPServer struct {
	port     string
	router   *Router
	dropRate float64
}

func NewUDPServer(port string, router *Router, dropRate float64) *UDPServer {
	return &UDPServer{
		port:     port,
		router:   router,
		dropRate: dropRate,
	}
}

func (s *UDPServer) Start() error {
	addr, err := net.ResolveUDPAddr("udp", ":"+s.port)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	fmt.Printf("Bank Server listening on %s (UDP)\n", s.port)

	for {
		buffer := make([]byte, 4096)
		n, clientAddr, err := conn.ReadFromUDP(buffer)
		if err != nil {
			continue
		}
		if s.dropRate > 0 && rand.Float64() < s.dropRate {
			fmt.Printf("[Simulated Drop] Dropped incoming request from %s\n", clientAddr.String())
			continue
		}
		s.router.HandleRequest(conn, clientAddr, buffer[:n])
	}
}
