#include "Service4_CheckBalanceControl.h"
#include <iostream>
#include <time.h>

Service4_CheckBalanceControl::Service4_CheckBalanceControl(INetworkTransport* t) : transport(t) {}

void Service4_CheckBalanceControl::execute(const std::string& name, int32_t account_num, const std::string& password) {
    Marshaller m;
    m.append_string(name); m.append_int(account_num); m.append_string(password);
    MessageHeader header = {transport->get_next_request_id(), OP_CHECK_BALANCE, transport->get_semantics(), static_cast<uint32_t>(m.get_size())};
    std::vector<uint8_t> req; header.marshal(req); req.insert(req.end(), m.get_buffer().begin(), m.get_buffer().end());
    std::vector<uint8_t> rep;
    if (transport->send_request(req, rep)) {
        size_t off = 0; MessageHeader h = MessageHeader::unmarshal(rep, off); Unmarshaller u(rep, off);
        if (h.opcode == OP_REPLY) std::cout << "Account Balance: " << u.read_float() << "\n";
        else std::cout << "ERROR: " << u.read_string() << "\n";
    }
}