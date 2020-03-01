cmake_minimum_required(VERSION 3.15)
project(multithreading)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(multithreading explore_key_words.cpp test_runner.h profile.h concurrent_map.cpp)