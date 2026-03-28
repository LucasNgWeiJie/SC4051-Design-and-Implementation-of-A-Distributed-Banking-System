package biz

import (
	"bank_server/data"
	"bytes"
	"encoding/binary"
)

type TransferService struct {
	repo *data.AccountRepository
}

func NewTransferService(repo *data.AccountRepository) *TransferService {
	return &TransferService{repo: repo}
}

func (s *TransferService) Execute(payload []byte) ([]byte, bool) {
	reader := bytes.NewReader(payload)
	
	// Read payload
	name, _ := readString(reader)
	fromAccID, _ := readInt(reader)
	password, _ := readString(reader)
	toAccID, _ := readInt(reader)
	amount, _ := readFloat(reader)

	// Execute transfer
	newBal, err := s.repo.Transfer(fromAccID, name, password, toAccID, amount)
	if err != nil {
		return createErrorPayload(err.Error()), true
	}
	
	// Return updated source balance
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.BigEndian, newBal)
	return buf.Bytes(), false
}
