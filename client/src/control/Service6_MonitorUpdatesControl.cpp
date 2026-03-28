#include "Service6_MonitorUpdatesControl.h"
#include <iostream>
#include <time.h>

Service6_MonitorUpdatesControl::Service6_MonitorUpdatesControl(INetworkTransport* t) : transport(t) {}

void Service6_MonitorUpdatesControl::execute(int32_t interval_seconds) {
    Marshaller m;
    m.append_int(interval_seconds);
    MessageHeader header = {transport->get_next_request_id(), OP_MONITOR_UPDATES, transport->get_semantics(), static_cast<uint32_t>(m.get_size())};
    std::vector<uint8_t> req; header.marshal(req); req.insert(req.end(), m.get_buffer().begin(), m.get_buffer().end());
    std::vector<uint8_t> rep;
    if (transport->send_request(req, rep)) {
        size_t off = 0; MessageHeader h = MessageHeader::unmarshal(rep, off); Unmarshaller u(rep, off);
        if (h.opcode == OP_REPLY) {
            std::cout << "Successfully registered for updates for " << interval_seconds << " seconds.\n";
            auto st = time(NULL);
            while (time(NULL) - st < interval_seconds) {
                int rem = (interval_seconds - (time(NULL) - st)) * 1000;
                if (rem <= 0) break;
                std::vector<uint8_t> up;
                if (transport->wait_for_message(up, rem)) {
                    size_t uf = 0; MessageHeader uh = MessageHeader::unmarshal(up, uf);
                    if (uh.opcode == OP_CALLBACK_UPDATE) {
                        Unmarshaller upd(up, uf); std::cout << "[UPDATE]: " << upd.read_string() << "\n";
                    }
                }
            }
            std::cout << "Monitoring interval expired.\n";
        } else std::cout << "ERROR: " << u.read_string() << "\n";
    }
}