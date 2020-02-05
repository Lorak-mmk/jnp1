cmake_minimum_required(VERSION 3.15)
project(images)

set(CMAKE_CXX_STANDARD 20)

set( CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O0 -fconcepts" )

include_directories(.)

set(SRC_FILES bmp.cc color.cc coordinate.cc images.cc)

add_executable(example example.cc ${SRC_FILES})

add_executable(test test.cc images.cc)