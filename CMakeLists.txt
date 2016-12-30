cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_COLOR_MAKEFILE off)
set(CMAKE_CXX_STANDARD 11)

project(stmake)

file(GLOB SOURCES "src/*.cpp")

set(EXECUTABLE_OUTPUT_PATH "bin")
add_executable(stmake ${SOURCES})
