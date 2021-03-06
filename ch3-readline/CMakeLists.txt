cmake_minimum_required(VERSION 3.5)

project(ch3-readline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ch3-readline OBJECT readline.cpp)
target_include_directories(ch3-readline INTERFACE .)

add_executable(ch3-readline-test test.cpp)
target_link_libraries(ch3-readline-test PRIVATE ch3-readline)

add_executable(ch3-readline-generate generate.cpp)
