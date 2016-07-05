cmake_minimum_required(VERSION 3.2)
project(tiwae)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -mavx")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXE_LINKER_FLAGS "-lboost_iostreams")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.58.0)

set(TIWAE_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")
set(TIWAE_TEST_PATH "${CMAKE_SOURCE_DIR}/test")

add_subdirectory(${TIWAE_SOURCE_PATH})



