cmake_minimum_required(VERSION 3.22)
project(Build_A_Shell)

set(CMAKE_CXX_STANDARD 14)

add_executable(Build_A_Shell src/main.cpp)

add_subdirectory(src)
add_subdirectory(include)