cmake_minimum_required(VERSION 3.15)
project(course_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(course_project main.cpp iterator_range.h parse.cpp parse.h search_server.cpp search_server.h test_runner.h profile.h)