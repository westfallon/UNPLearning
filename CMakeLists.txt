cmake_minimum_required(VERSION 3.20)
project(UNPLearning)

include_directories(/usr/local/include)

set(CMAKE_CXX_STANDARD 11)

add_executable(UNPLearning main.c c3/byteorder.c c4/daytime_tcp_srv.c c4/daytime_tcp_cli.c c4/sockfd_to_family.c c5/echo_tcp_srv.c c5/echo_tcp_cli.c c6/echo_tcp_cli_select.c c6/echo_tcp_srv_select.c)

target_link_libraries(UNPLearning unp)