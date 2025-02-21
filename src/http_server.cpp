#include "http_server.hpp"

#include <spdlog/spdlog.h>
#include <winsock2.h>

#include <fstream>
#include <thread>

#include "util/socket.hpp"

constexpr std::size_t kBufferSize = 1024;

HttpServer::HttpServer(std::uint16_t port, std::string file_path)
    : port_(port),
      file_(file_path),
      server_socket_(util::Socket(util::Socket::family_t::inet,
                                  util::Socket::specification_t::stream,
                                  util::Socket::protocol_t::tcp)) {
  std::ifstream file(file_);
  file_content_ = std::string((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
  file.close();
  spdlog::info("Server config: Port {} | File size {}", port_,
               file_content_.size());
}

HttpServer::~HttpServer() {}

void HttpServer::HandleRequest(util::Socket& clientSocket,
                               const std::vector<std::uint8_t>& data) {
  std::string request(data.begin(), data.end());
  spdlog::info("Request: {}", request);
  if (request.find("GET") == std::string::npos) {
    SendResponse(clientSocket, "HTTP/1.1 400 Bad Request\r\n\r\n", "");
    return;
  }

  SendResponse(clientSocket, "HTTP/1.1 200 OK\r\n\r\n", file_content_);
}

void HttpServer::SendResponse(util::Socket& clientSocket,
                              const std::string& headers,
                              const std::string& content) {
  auto data = headers + content;
  clientSocket.Write({data.begin(), data.end()});
}

void HttpServer::Start() {
  spdlog::info("Starting server");
  server_socket_.Bind("127.0.0.1", port_);
  server_socket_.Listen();

  while (true) {
    auto client_socket = server_socket_.Accept();
    spdlog::info("Client connected:");
    auto data = server_socket_.Read(kBufferSize);
    std::thread(&HttpServer::HandleRequest, this, client_socket, data).detach();
  }
}