cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0076 NEW)

project(experiment)

add_definitions(-Weverything -Wno-poison-system-directories -Wno-padded -g)
add_subdirectory(src)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
