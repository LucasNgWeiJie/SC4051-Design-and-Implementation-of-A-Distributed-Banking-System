package common

const (
	OpOpenAccount     uint8 = 1
	OpCloseAccount    uint8 = 2
	OpDepositWithdraw uint8 = 3
	OpCheckBalance    uint8 = 4
	OpTransfer        uint8 = 5
	OpMonitorUpdates  uint8 = 6
	OpReply           uint8 = 7
	OpError           uint8 = 8
	OpCallbackUpdate  uint8 = 9

	SemAtLeastOnce uint8 = 0
	SemAtMostOnce  uint8 = 1
)

type MessageHeader struct {
	RequestID     uint32
	Opcode        uint8
	Semantics     uint8
	PayloadLength uint32
}

type Account struct {
	Number   int32
	Name     string
	Password string
	Currency uint8
	Balance  float32
}
