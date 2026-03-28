package biz

import (
	"bank_server/data"
	"bytes"
)

type MonitorUpdatesService struct {
	repo *data.AccountRepository
}

func NewMonitorUpdatesService(repo *data.AccountRepository) *MonitorUpdatesService {
	return &MonitorUpdatesService{repo: repo}
}

func (s *MonitorUpdatesService) Execute(payload []byte, clientAddr string, registerFunc func(string, int32)) ([]byte, bool) {
	reader := bytes.NewReader(payload)
	
	// Parse requested monitor interval
	interval, _ := readInt(reader)
	
	// Register the callback destination on the Router
	registerFunc(clientAddr, interval)
	
	return []byte{}, false
}
