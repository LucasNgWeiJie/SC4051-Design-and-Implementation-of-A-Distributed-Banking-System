package biz

import (
	"bank_server/data"
	"bytes"
	"encoding/binary"
)

type OpenAccountService struct {
	repo *data.AccountRepository
}

func NewOpenAccountService(repo *data.AccountRepository) *OpenAccountService {
	return &OpenAccountService{repo: repo}
}

func (s *OpenAccountService) Execute(payload []byte) ([]byte, bool) {
	reader := bytes.NewReader(payload)
	
	// Read credentials
	name, _ := readString(reader)
	password, _ := readString(reader)
	currency, _ := reader.ReadByte()
	balance, _ := readFloat(reader)

	// Persist account creation
	accID := s.repo.CreateAccount(name, password, currency, balance)
	
	// Return new account ID
	resp := make([]byte, 4)
	binary.BigEndian.PutUint32(resp, uint32(accID))
	return resp, false
}
