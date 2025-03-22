cmake_minimum_required(VERSION 3.10)
project(combinatorial_algorithms)

set(CMAKE_CXX_STANDARD 14)

include_directories(labs/3-lab/class-graph)

add_executable(combinatorial_algorithms
    labs/3-lab/class-graph/graph.cpp
    labs/3-lab/class-graph/graph.h
    labs/3-lab/description.md
    labs/3-lab/main.cpp)
