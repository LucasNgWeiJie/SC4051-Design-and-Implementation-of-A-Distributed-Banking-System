#include "Service1_OpenAccountControl.h"
#include <iostream>
#include <time.h>

Service1_OpenAccountControl::Service1_OpenAccountControl(INetworkTransport* t) : transport(t) {}

void Service1_OpenAccountControl::execute(const std::string& name, const std::string& password, Currency currency, float balance) {
    Marshaller m;
    m.append_string(name); m.append_string(password); m.append_byte(currency); m.append_float(balance);
    MessageHeader header = {transport->get_next_request_id(), OP_OPEN_ACCOUNT, transport->get_semantics(), static_cast<uint32_t>(m.get_size())};
    std::vector<uint8_t> req; header.marshal(req); req.insert(req.end(), m.get_buffer().begin(), m.get_buffer().end());
    std::vector<uint8_t> rep;
    if (transport->send_request(req, rep)) {
        size_t off = 0; MessageHeader h = MessageHeader::unmarshal(rep, off); Unmarshaller u(rep, off);
        if (h.opcode == OP_REPLY) std::cout << "Account created successfully. Account Number: " << u.read_int() << "\n";
        else std::cout << "ERROR: " << u.read_string() << "\n";
    }
}