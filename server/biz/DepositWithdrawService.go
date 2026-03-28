package biz

import (
	"bank_server/data"
	"bytes"
	"encoding/binary"
)

type DepositWithdrawService struct {
	repo *data.AccountRepository
}

func NewDepositWithdrawService(repo *data.AccountRepository) *DepositWithdrawService {
	return &DepositWithdrawService{repo: repo}
}

func (s *DepositWithdrawService) Execute(payload []byte) ([]byte, bool) {
	reader := bytes.NewReader(payload)
	
	// Read payload
	name, _ := readString(reader)
	accID, _ := readInt(reader)
	password, _ := readString(reader)
	currency, _ := reader.ReadByte()
	amount, _ := readFloat(reader)

	// Execute operation via repository
	newBal, err := s.repo.UpdateBalance(accID, name, password, currency, amount)
	if err != nil {
		return createErrorPayload(err.Error()), true
	}
	
	// Return updated balance
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.BigEndian, newBal)
	return buf.Bytes(), false
}
