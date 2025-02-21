
#include <spdlog/spdlog.h>

#include <cstdint>
#include <cxxopts.hpp>
#include <iostream>

#include "http_server.hpp"

int main(int argc, char* argv[]) {
  cxxopts::Options options("simple_http_server", "Simple HTTP server");

  options.add_options()("h,help", "Print help")(
      "p,port", "Port to listen", cxxopts::value<std::uint16_t>())(
      "f,file", "File to server", cxxopts::value<std::string>());

  try {
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    if (!(result.count("port") && result.count("file"))) {
      throw cxxopts::exceptions::missing_argument("port and file");
    }

    try {
      HttpServer server(result["port"].as<std::uint16_t>(),
                        result["file"].as<std::string>());
      server.Start();
    } catch (const std::exception& e) {
      spdlog::error(e.what());
      return 1;
    }
  } catch (const cxxopts::exceptions::exception& e) {
    spdlog::error(e.what());
    return 1;
  }

  return 0;
}