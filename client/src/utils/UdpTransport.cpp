#include "../include/UdpTransport.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

UdpTransport::UdpTransport(const std::string &server_ip, int port,
                           Semantics sem)
    : next_request_id(1), semantics(sem), timeout_ms(2000) {

  // Initialize Winsock
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "WSAStartup failed\n";
    exit(1);
  }

  // Create UDP socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == INVALID_SOCKET) {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
    WSACleanup();
    exit(1);
  }

  // Server address
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  inet_pton(AF_INET, server_ip.c_str(), &servaddr.sin_addr);
}

UdpTransport::~UdpTransport() {
  closesocket(sockfd);
  WSACleanup();
}

bool UdpTransport::send_request(const std::vector<uint8_t> &request,
                                std::vector<uint8_t> &reply) {
  int max_retries = 3;

  for (int retry = 0; retry < max_retries; ++retry) {
    sendto(sockfd, reinterpret_cast<const char *>(request.data()),
           static_cast<int>(request.size()), 0,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    // Use select instead of poll for Windows compatibility
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = select(0, &readfds, NULL, NULL, &tv);
    if (ret > 0) {
      reply.resize(4096);
      int len = sizeof(servaddr);
      int n = recvfrom(sockfd, reinterpret_cast<char *>(reply.data()),
                       static_cast<int>(reply.size()), 0,
                       (struct sockaddr *)&servaddr, &len);
      if (n == SOCKET_ERROR) {
        std::cerr << "recvfrom failed: " << WSAGetLastError() << "\n";
        return false;
      }
      reply.resize(n);
      return true;
    } else if (ret == 0) {
      std::cout << "Timeout, retrying (" << retry + 1 << "/" << max_retries
                << ")...\n";
    } else {
      std::cerr << "select error: " << WSAGetLastError() << "\n";
      return false;
    }
  }
  std::cerr << "Failed to receive reply after maximum retries.\n";
  return false;
}

bool UdpTransport::wait_for_message(std::vector<uint8_t> &msg, int timeout_ms) {
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(sockfd, &readfds);

  struct timeval tv;
  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (timeout_ms % 1000) * 1000;

  int ret = select(0, &readfds, NULL, NULL, &tv);
  if (ret > 0) {
    msg.resize(4096);
    int n = recvfrom(sockfd, reinterpret_cast<char *>(msg.data()),
                     static_cast<int>(msg.size()), 0, NULL, NULL);
    if (n == SOCKET_ERROR) return false;
    msg.resize(n);
    return true;
  }
  return false;
}