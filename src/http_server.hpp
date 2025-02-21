#pragma once

#include <cstdint>
#include <string>

class HttpServer {
 private:
  std::uint16_t port_;
  std::string file_;

 public:
  HttpServer() = delete;
  HttpServer(std::uint16_t port, std::string file);

  void Start();
};