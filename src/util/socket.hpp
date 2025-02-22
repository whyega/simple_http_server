#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace util {
class Socket {
 private:
  using socket_handle_t = std::uintptr_t;
  socket_handle_t socket_;

 public:
  enum class family_t { inet = 2, inet6 = 6 };
  enum class specification_t { stream = 1 };
  enum class protocol_t { tcp, udp };

  Socket() = delete;
  Socket(family_t family, specification_t specification, protocol_t protocol);
  ~Socket();
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;
  Socket(Socket&& other) noexcept;
  Socket& operator=(Socket&& other) noexcept;

  void Bind(const std::string& address, std::uint16_t port);
  void Listen();
  Socket Accept();
  std::vector<std::uint8_t> Read(const std::size_t size);
  void Write(const std::vector<std::uint8_t>& data);
};
}  // namespace util
