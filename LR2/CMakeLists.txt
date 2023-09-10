cmake_minimum_required(VERSION 3.26)
project(LR2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(SOURCE_FILES main.cpp)
add_executable(LR2 main.cpp)