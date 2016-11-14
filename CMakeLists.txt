cmake_minimum_required(VERSION 3.3.1)
project(TetrahedronPuzzle CXX)
add_executable(TetrahedronPuzzle main.cpp)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++1z")

target_link_libraries(TetrahedronPuzzle glfw GL GLU)
