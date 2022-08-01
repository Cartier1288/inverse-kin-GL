cmake_minimum_required(VERSION 3.12)

project(inversekin)

add_definitions("-std=c++17")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# conan setup
include(${inversekin_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()
message( STATUS "included ${inversekin_SOURCE_DIR}/build/conanbuildinfo.cmake")


add_subdirectory(src)
