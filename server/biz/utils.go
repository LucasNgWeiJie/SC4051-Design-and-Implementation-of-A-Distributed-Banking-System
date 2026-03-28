package biz

import (
	"bytes"
	"encoding/binary"
)

func readString(r *bytes.Reader) (string, error) {
	var l uint16
	err := binary.Read(r, binary.BigEndian, &l)
	if err != nil {
		return "", err
	}
	buf := make([]byte, l)
	_, err = r.Read(buf)
	return string(buf), err
}
func readInt(r *bytes.Reader) (int32, error) {
	var val int32
	err := binary.Read(r, binary.BigEndian, &val)
	return val, err
}
func readFloat(r *bytes.Reader) (float32, error) {
	var val float32
	err := binary.Read(r, binary.BigEndian, &val)
	return val, err
}
func createErrorPayload(msg string) []byte {
	buf := new(bytes.Buffer)
	l := uint16(len(msg))
	binary.Write(buf, binary.BigEndian, l)
	buf.WriteString(msg)
	return buf.Bytes()
}
