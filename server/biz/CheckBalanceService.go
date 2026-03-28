package biz

import (
	"bank_server/data"
	"bytes"
	"encoding/binary"
)

type CheckBalanceService struct {
	repo *data.AccountRepository
}

func NewCheckBalanceService(repo *data.AccountRepository) *CheckBalanceService {
	return &CheckBalanceService{repo: repo}
}

func (s *CheckBalanceService) Execute(payload []byte) ([]byte, bool) {
	reader := bytes.NewReader(payload)
	
	// Read payload
	name, _ := readString(reader)
	accID, _ := readInt(reader)
	password, _ := readString(reader)

	// Fetch account
	acc, err := s.repo.GetAccount(accID, name, password)
	if err != nil {
		return createErrorPayload(err.Error()), true
	}
	
	// Return balance
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.BigEndian, acc.Balance)
	return buf.Bytes(), false
}
