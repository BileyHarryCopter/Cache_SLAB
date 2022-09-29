cmake_minimum_required(VERSION 3.14)

project(slow_lfuda)

include_directories(../include/)
add_executable(lfuda ./slow_lfuda.cpp)
