cmake_minimum_required(VERSION 3.8)
project(kdtree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        .idea/codeStyles/Project.xml
        .idea/kdtree.iml
        .idea/misc.xml
        .idea/modules.xml
        .idea/vcs.xml
        .idea/workspace.xml
        cmake-build-debug/CMakeCache.txt
        cmake_modules/FindSFML.cmake
        .gitignore
        CMakeLists.txt
        kdtree.cpp
        kdtree.h
        main.cpp
        README.md)

add_executable(kdtree ${SOURCE_FILES})