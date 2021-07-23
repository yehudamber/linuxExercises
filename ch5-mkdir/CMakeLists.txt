cmake_minimum_required(VERSION 3.5)

project(ch5-mkdir LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(../cmake/warnings.cmake)

add_executable(ch5-mkdir controller.cpp
                         main.cpp
                         makedir.cpp)
set_project_warnings(ch5-mkdir)
