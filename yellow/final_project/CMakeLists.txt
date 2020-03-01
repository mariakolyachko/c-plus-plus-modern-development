cmake_minimum_required(VERSION 3.15)
project(final_project)

set(CMAKE_CXX_STANDARD 14)

add_executable(final_project main.cpp date.h date.cpp database.h database.cpp node.h condition_parser.h condition_parser.cpp token.cpp node.cpp test_db.h test_db.h)