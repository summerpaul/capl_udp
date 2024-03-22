cmake_minimum_required(VERSION 3.0.2)
project(capl_udp)

# 编译选项
option(BUILD_DEMO "build capl udp demo" ON)

option(BUILD_EXAMPLE "build udp server client examples" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include_directories(include)



add_library(capl_udp_client src/capl_udp_client.cpp)

if(BUILD_DEMO)
    add_executable(capl_udp_demo demo/capl_udp_demo.cpp)
    target_link_libraries(capl_udp_demo capl_udp_client)
endif(BUILD_DEMO)

if(BUILD_EXAMPLE)
    add_executable(client exmaples/client.c)
    add_executable(server exmaples/server.c)

endif(BUILD_EXAMPLE)