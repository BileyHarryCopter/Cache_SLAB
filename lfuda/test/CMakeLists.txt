cmake_minimum_required(VERSION 3.14)

project(slow_lfuda)

include_directories(../include/)
add_executable(lfuda src/slow_lfuda.cpp)
