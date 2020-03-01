cmake_minimum_required(VERSION 3.15)
project(linear_containers)

set(CMAKE_CXX_STANDARD 17)

add_executable(linear_containers airport_counter.cpp test_runner.h profile.h text_editor.cpp http_request.h stats.h serve_stats.cpp stats.cpp)