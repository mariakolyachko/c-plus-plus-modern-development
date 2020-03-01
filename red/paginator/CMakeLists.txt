cmake_minimum_required(VERSION 3.15)
project(paginator)

set(CMAKE_CXX_STANDARD 17)

add_executable(paginator main.cpp paginator.cpp test_runner.h)