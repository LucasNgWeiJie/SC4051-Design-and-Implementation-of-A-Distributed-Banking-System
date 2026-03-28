#include "Service2_CloseAccountControl.h"
#include <iostream>
#include <time.h>

Service2_CloseAccountControl::Service2_CloseAccountControl(INetworkTransport* t) : transport(t) {}

void Service2_CloseAccountControl::execute(const std::string& name, int32_t account_num, const std::string& password) {
    Marshaller m;
    m.append_string(name); m.append_int(account_num); m.append_string(password);
    MessageHeader header = {transport->get_next_request_id(), OP_CLOSE_ACCOUNT, transport->get_semantics(), static_cast<uint32_t>(m.get_size())};
    std::vector<uint8_t> req; header.marshal(req); req.insert(req.end(), m.get_buffer().begin(), m.get_buffer().end());
    std::vector<uint8_t> rep;
    if (transport->send_request(req, rep)) {
        size_t off = 0; MessageHeader h = MessageHeader::unmarshal(rep, off); Unmarshaller u(rep, off);
        if (h.opcode == OP_REPLY) std::cout << "Account closed successfully.\n";
        else std::cout << "ERROR: " << u.read_string() << "\n";
    }
}