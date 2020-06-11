cmake_minimum_required(VERSION 3.15)
project(lab3_clion)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab3_clion main.cpp AVLTree.hpp PriorityQueue.hpp)
add_executable(test test.cpp)
add_executable(benchmark Benchmark.cpp)