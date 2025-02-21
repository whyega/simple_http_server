#include "socket.hpp"

#include <stdexcept>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

namespace util {
Socket::Socket(family_t family, specification_t specification,
               protocol_t protocol) {
  socket_ = socket(static_cast<int>(family), static_cast<int>(specification),
                   static_cast<int>(protocol));
}

Socket::~Socket() {
  if (socket_ != 0) {
#ifdef _WIN32
    closesocket(socket_);
    WSACleanup();
#else
    close(socket_);
#endif
  }
}

bool Socket::Bind(const std::string& address, std::uint16_t& port) {
  struct sockaddr_in server_address;
  server_address.sin_family = static_cast<int>(family_t::inet);
  server_address.sin_port = htons(port);

  if (inet_pton(AF_INET, address.c_str(), &server_address.sin_addr) <= 0) {
    return false;
  }

  if (bind(socket_, (struct sockaddr*)&server_address, sizeof(server_address)) <
      0) {
    return false;
  }

  port = ntohs(server_address.sin_port);
  return true;
}

void Socket::Listen() {
  if (listen(socket_, SOMAXCONN) < 0) {
    throw std::runtime_error("Failed to listen");
  }
}

Socket Socket::Accept() {
  struct sockaddr_in client_address;
  socklen_t client_address_size = sizeof(client_address);
  auto client_socket =
      accept(socket_, (struct sockaddr*)&client_address, &client_address_size);
  if (client_socket < 0) {
    throw std::runtime_error("Failed to accept");
  }
  Socket client(family_t::inet, specification_t::stream, protocol_t::tcp);
  client.socket_ = client_socket;
  return client;
}

std::vector<std::uint8_t> Socket::Read(const std::size_t size) {
  std::vector<std::uint8_t> buffer(size);
  auto bytes_read =
      recv(socket_, reinterpret_cast<char*>(buffer.data()), size, 0);
  if (bytes_read < 0) {
    throw std::runtime_error("Failed to read");
  }
  buffer.resize(bytes_read);
  return buffer;
}

void Socket::Write(const std::vector<std::uint8_t>& data) {
  auto bytes_written =
      send(socket_, reinterpret_cast<const char*>(data.data()), data.size(), 0);
  if (bytes_written < 0) {
    throw std::runtime_error("Failed to write");
  }
}
};  // namespace util