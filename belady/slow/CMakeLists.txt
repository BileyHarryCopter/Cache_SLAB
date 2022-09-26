cmake_minimum_required(VERSION 3.14)

project(slow_belady)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(../include/)

add_executable(belady src/slow_belady.cpp)