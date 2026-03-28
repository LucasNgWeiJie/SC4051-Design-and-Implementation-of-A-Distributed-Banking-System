#ifndef INETWORK_TRANSPORT_H
#define INETWORK_TRANSPORT_H

#include "Protocol.h"
#include <vector>

class INetworkTransport {
public:
    virtual ~INetworkTransport() = default;
    
    virtual bool send_request(const std::vector<uint8_t>& request, std::vector<uint8_t>& reply) = 0;
    virtual bool wait_for_message(std::vector<uint8_t>& msg, int timeout_ms) = 0;

    virtual uint32_t get_next_request_id() = 0;
    virtual Semantics get_semantics() const = 0;
};

#endif
