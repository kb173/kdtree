cmake_minimum_required(VERSION 3.10)
project(kdtree)

set(CMAKE_CXX_STANDARD 11)

add_executable(kdtree main.cpp cmake-build-debug/tree.cpp cmake-build-debug/tree.h kdtree.cpp kdtree.h)