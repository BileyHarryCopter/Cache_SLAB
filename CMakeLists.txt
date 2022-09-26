cmake_minimum_required(VERSION 3.14)

project (cache)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(lfuda)
add_subdirectory(belady)