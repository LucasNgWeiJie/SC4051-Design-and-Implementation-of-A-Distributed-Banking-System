package biz

import (
	"bank_server/data"
	"bytes"
)

type CloseAccountService struct {
	repo *data.AccountRepository
}

func NewCloseAccountService(repo *data.AccountRepository) *CloseAccountService {
	return &CloseAccountService{repo: repo}
}

func (s *CloseAccountService) Execute(payload []byte) ([]byte, bool) {
	reader := bytes.NewReader(payload)
	
	// Read credentials
	name, _ := readString(reader)
	accID, _ := readInt(reader)
	password, _ := readString(reader)

	// Delete from storage
	err := s.repo.DeleteAccount(accID, name, password)
	if err != nil {
		return createErrorPayload(err.Error()), true
	}
	return []byte{}, false
}
