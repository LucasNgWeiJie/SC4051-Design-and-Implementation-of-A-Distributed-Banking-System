#ifndef UDP_TRANSPORT_H
#define UDP_TRANSPORT_H

#include "INetworkTransport.h"
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class UdpTransport : public INetworkTransport {
  int sockfd;
  struct sockaddr_in servaddr;
  uint32_t next_request_id;
  Semantics semantics;
  int timeout_ms;

public:
  UdpTransport(const std::string &server_ip, int port, Semantics sem);
  ~UdpTransport();

  bool send_request(const std::vector<uint8_t> &request,
                    std::vector<uint8_t> &reply) override;
  bool wait_for_message(std::vector<uint8_t> &msg, int timeout_ms) override;

  uint32_t get_next_request_id() override { return next_request_id++; }
  Semantics get_semantics() const override { return semantics; }
};

#endif
