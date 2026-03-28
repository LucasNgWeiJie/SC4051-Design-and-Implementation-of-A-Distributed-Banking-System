#include "../include/UdpTransport.h"
#include <arpa/inet.h>
#include <iostream>
#include <poll.h>
#include <unistd.h>

UdpTransport::UdpTransport(const std::string &server_ip, int port,
                           Semantics sem)
    : next_request_id(1), semantics(sem), timeout_ms(2000) {

  // Create UDP Socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    std::cerr << "Socket creation failed\n";
    exit(1);
  }

  // server address logic
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());
}

UdpTransport::~UdpTransport() { close(sockfd); }

bool UdpTransport::send_request(const std::vector<uint8_t> &request,
                                std::vector<uint8_t> &reply) {
  int max_retries = 3;

  // Attempt to send data and wait for response with a set number of max retries
  for (int retry = 0; retry < max_retries; ++retry) {
    // Send request array to the target server
    sendto(sockfd, request.data(), request.size(), 0,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    struct pollfd fd;
    fd.fd = sockfd;
    fd.events = POLLIN;

    // Wait and check if the poll times out
    int ret = poll(&fd, 1, timeout_ms);
    if (ret > 0) {
      reply.resize(4096);
      socklen_t len = sizeof(servaddr);
      // Receive bytes
      int n = recvfrom(sockfd, reply.data(), reply.size(), 0,
                       (struct sockaddr *)&servaddr, &len);
      reply.resize(n);
      return true;
    } else if (ret == 0) {
      std::cout << "Timeout, retrying (" << retry + 1 << "/" << max_retries
                << ")...\n";
    } else {
      std::cerr << "Poll error\n";
      return false;
    }
  }
  std::cerr << "Failed to receive reply after maximum retries.\n";
  return false;
}

bool UdpTransport::wait_for_message(std::vector<uint8_t> &msg, int timeout_ms) {
  struct pollfd fd;
  fd.fd = sockfd;
  fd.events = POLLIN;

  int ret = poll(&fd, 1, timeout_ms);
  if (ret > 0) {
    msg.resize(4096);
    int n = recvfrom(sockfd, msg.data(), msg.size(), 0, NULL, NULL);
    msg.resize(n);
    return true;
  }
  return false;
}
