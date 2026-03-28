#include "Service3_DepositWithdrawControl.h"
#include <iostream>
#include <time.h>

Service3_DepositWithdrawControl::Service3_DepositWithdrawControl(INetworkTransport* t) : transport(t) {}

void Service3_DepositWithdrawControl::execute(const std::string& name, int32_t account_num, const std::string& password, Currency currency, float amount) {
    Marshaller m;
    m.append_string(name); m.append_int(account_num); m.append_string(password); m.append_byte(currency); m.append_float(amount);
    MessageHeader header = {transport->get_next_request_id(), OP_DEPOSIT_WITHDRAW, transport->get_semantics(), static_cast<uint32_t>(m.get_size())};
    std::vector<uint8_t> req; header.marshal(req); req.insert(req.end(), m.get_buffer().begin(), m.get_buffer().end());
    std::vector<uint8_t> rep;
    if (transport->send_request(req, rep)) {
        size_t off = 0; MessageHeader h = MessageHeader::unmarshal(rep, off); Unmarshaller u(rep, off);
        if (h.opcode == OP_REPLY) std::cout << "Transaction successful. New Balance: " << u.read_float() << "\n";
        else std::cout << "ERROR: " << u.read_string() << "\n";
    }
}