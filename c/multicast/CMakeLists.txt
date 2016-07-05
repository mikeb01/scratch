cmake_minimum_required(VERSION 3.3)
project(scratch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE main.cpp mcastserver.cpp mcastclient.cpp multicast.cpp)

#add_executable(scratch ${SOURCE_FILES})
add_executable(nets nets.cpp)
add_executable(mcastclient mcastclient.cpp multicast.cpp multicast.h)
add_executable(mcastserver mcastserver.cpp multicast.cpp multicast.h)
add_executable(udp_multicast udp_multicast.cpp)
add_executable(udp_multicast2 udp_multicast2.cpp)
add_executable(udp_multicast3 udp_multicast3.cpp)