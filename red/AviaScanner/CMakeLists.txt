cmake_minimum_required(VERSION 3.15)
project(AviaScanner)

set(CMAKE_CXX_STANDARD 17)

add_executable(AviaScanner main.cpp airline_ticket.cpp sort_by.cpp update_fields.cpp)