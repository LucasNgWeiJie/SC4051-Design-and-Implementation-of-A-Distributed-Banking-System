#include "Service5_TransferControl.h"
#include <iostream>
#include <time.h>

Service5_TransferControl::Service5_TransferControl(INetworkTransport* t) : transport(t) {}

void Service5_TransferControl::execute(const std::string& name, int32_t from_account, const std::string& password, int32_t to_account, float amount) {
    Marshaller m;
    m.append_string(name); m.append_int(from_account); m.append_string(password); m.append_int(to_account); m.append_float(amount);
    MessageHeader header = {transport->get_next_request_id(), OP_TRANSFER, transport->get_semantics(), static_cast<uint32_t>(m.get_size())};
    std::vector<uint8_t> req; header.marshal(req); req.insert(req.end(), m.get_buffer().begin(), m.get_buffer().end());
    std::vector<uint8_t> rep;
    if (transport->send_request(req, rep)) {
        size_t off = 0; MessageHeader h = MessageHeader::unmarshal(rep, off); Unmarshaller u(rep, off);
        if (h.opcode == OP_REPLY) std::cout << "Transfer successful. Source New Balance: " << u.read_float() << "\n";
        else std::cout << "ERROR: " << u.read_string() << "\n";
    }
}