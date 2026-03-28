#ifndef UDP_TRANSPORT_H
#define UDP_TRANSPORT_H

#include "INetworkTransport.h"
#include <string>
#include <vector>
#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib") // link Winsock
  using socklen_t = int;             // Windows uses int instead of socklen_t
#else
  #include <netinet/in.h>
  #include <sys/socket.h>
#endif

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
