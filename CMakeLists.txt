cmake_minimum_required(VERSION 3.20)
project(UNPLearning)

include_directories(/usr/local/include)

set(CMAKE_CXX_STANDARD 11)

add_executable(UNPLearning main.c c3/byteorder.c c4/daytime_tcp_srv.c)

target_link_libraries(UNPLearning unp)