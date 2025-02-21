#pragma once

#include <cstdint>
#include <string>

#include "util/socket.hpp"

class HttpServer {
 private:
  util::Socket server_socket_;
  std::uint16_t port_;
  std::string file_content_;

  void HandleRequest(util::Socket& clientSocket,
                     const std::vector<std::uint8_t>& data);
  void SendResponse(util::Socket& clientSocket, const std::string& headers,
                    const std::string& content);

 public:
  HttpServer() = delete;
  HttpServer(std::uint16_t port, std::string file_path);

  ~HttpServer();

  void Start();
};