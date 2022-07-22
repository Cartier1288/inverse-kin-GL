project(inversekin)
cmake_minimum_required(VERSION 2.8.12)

add_definitions("-std=c++17")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

# conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

message( STATUS "included ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")

add_subdirectory(src)
