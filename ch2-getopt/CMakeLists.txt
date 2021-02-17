cmake_minimum_required(VERSION 3.5)

project(ch2-getopt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ch2-getopt OBJECT getopt.cpp)
target_include_directories(ch2-getopt INTERFACE .)

add_executable(ch2-getopt-test1 test1.cpp)
target_link_libraries(ch2-getopt-test1 PRIVATE ch2-getopt)
