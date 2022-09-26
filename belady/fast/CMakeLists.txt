cmake_minimum_required(VERSION 3.14)

project(belady)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(../include/)

add_executable(belady src/belady.cpp)