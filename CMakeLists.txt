cmake_minimum_required(VERSION 4.2)
project(filter)

set(CMAKE_CXX_STANDARD 20)

add_executable(filter main.cpp
        include/bmp.hpp
        filehandler.cpp
        include/filehandler.hpp
        include/filter.hpp
        filter.cpp)
