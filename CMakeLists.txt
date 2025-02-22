cmake_minimum_required(VERSION 3.15)
project(simple_http_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor)

list(APPEND SIMPLE_HTTP_SERVER_SRC
    # entry point
    src/main.cpp

    src/util/socket.cpp
    src/util/thread_pool.cpp
    src/http_server.cpp
)

add_executable(simple_http_server ${SIMPLE_HTTP_SERVER_SRC})
target_link_libraries(simple_http_server PRIVATE 
    spdlog::spdlog
    cxxopts::cxxopts
)