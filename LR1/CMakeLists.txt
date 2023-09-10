cmake_minimum_required(VERSION 3.26)
project(LR1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(SOURCE_FILES main.cpp input.txt)
add_executable(LR1 main.cpp)
