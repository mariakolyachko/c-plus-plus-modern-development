cmake_minimum_required(VERSION 3.15)
project(yellow)

set(CMAKE_CXX_STANDARD 14)

add_executable(yellow main.cpp bus_manager.h functions.cpp test_runner.h test_runner.cpp output.h assertation.h assertation.cpp tests.h tests.cpp query.h query.cpp responses.h responses.cpp bus_manager.cpp)