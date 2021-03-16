cmake_minimum_required(VERSION 3.14)

project(ch4-perror LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ch4-perror OBJECT perror.cpp)
target_include_directories(ch4-perror INTERFACE .)

add_executable(ch4-perror-test test.cpp)
target_link_libraries(ch4-perror-test PRIVATE ch4-perror)
