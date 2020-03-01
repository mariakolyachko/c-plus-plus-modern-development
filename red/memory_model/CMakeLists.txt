cmake_minimum_required(VERSION 3.15)
project(memory_model)

set(CMAKE_CXX_STANDARD 17)

add_executable(memory_model object_pool.cpp test_runner.h simple_vector.h simple_vector.cpp linked_list.cpp)