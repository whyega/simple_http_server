#include "http_server.hpp"

#include <spdlog/spdlog.h>

HttpServer::HttpServer(std::uint16_t port, std::string file)
    : port_(port), file_(file) {}

void HttpServer::Start() { spdlog::info("Starting server on port {}", port_); }