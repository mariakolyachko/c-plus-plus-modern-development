cmake_minimum_required(VERSION 3.15)
project(move_semantics)

set(CMAKE_CXX_STANDARD 17)

add_executable(move_semantics josephus_permutation.cpp test_runner.h split_into_sentances.cpp merge_sort.cpp priority_collection.cpp)