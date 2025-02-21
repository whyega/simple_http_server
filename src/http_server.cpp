#include "http_server.hpp"

#include <spdlog/spdlog.h>
#include <winsock2.h>

#include <fstream>
#include <thread>

#include "util/socket.hpp"

constexpr std::size_t kBufferSize = 1024;

HttpServer::HttpServer(std::uint16_t port, std::string file_path)
    : port_(port),
      server_socket_(util::Socket(util::Socket::family_t::inet,
                                  util::Socket::specification_t::stream,
                                  util::Socket::protocol_t::tcp)) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
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
  //   TODO: add post
  if (request.find("GET") == std::string::npos) {
    SendResponse(clientSocket, "HTTP/1.1 400 Bad Request\r\n\r\n", "");
    return;
  }

  SendResponse(clientSocket, "HTTP/1.1 200 OK\r\n\r\n", file_content_);
}

void HttpServer::SendResponse(util::Socket& clientSocket,
                              const std::string& headers,
                              const std::string& content) {
  spdlog::info("Send response: headers -> {} | content -> {}", headers,
               content);
  auto data = headers + content;
  clientSocket.Write({data.begin(), data.end()});
}
void HttpServer::Start() {
  spdlog::info("Starting server");
  try {
    server_socket_.Bind("127.0.0.1", port_);
    server_socket_.Listen();
  } catch (const std::exception& e) {
    spdlog::error(e.what());
    return;
  }

  while (true) {
    auto client_socket = server_socket_.Accept();
    spdlog::info("Client connected");
    try {
      auto data = client_socket.Read(kBufferSize);
      std::thread(std::bind(&HttpServer::HandleRequest, this,
                            std::ref(client_socket), std::ref(data)))
          .detach();
    } catch (const std::exception& e) {
      spdlog::error("Error handling client: {}", e.what());
    }
  }
}