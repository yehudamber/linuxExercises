cmake_minimum_required(VERSION 3.5)

project(ch5-catdir LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(../cmake/warnings.cmake)

add_executable(ch5-catdir main.cpp
                          controller.cpp
                          directory.cpp)
set_project_warnings(ch5-catdir)
