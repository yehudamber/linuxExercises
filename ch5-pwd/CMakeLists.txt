cmake_minimum_required(VERSION 3.5)

project(ch5-pwd LANGUAGES CXX)

include(../cmake/warnings.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ch5-pwd main.cpp)
set_project_warnings(ch5-pwd)
