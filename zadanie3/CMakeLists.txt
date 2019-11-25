cmake_minimum_required(VERSION 3.14)
project(jnp3)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.45.0)

set (CMAKE_CXX_FLAGS "-Wall -Wextra -O2 -std=c++17")

include_directories(.)

add_library(fibo fibo.cc fibo.h)

add_executable(base_test fibo tests/base_test.cc)
add_executable(addition fibo tests/addition_tests.cc)
add_executable(print fibo tests/print_tests.cpp)
add_executable(fb_test fibo tests/fb_test.cpp)
add_executable(ctors_and_ops fibo tests/ctors_and_ops.cpp)
