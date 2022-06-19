cmake_minimum_required(VERSION 2.8.12)
project(inversekin)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(main main.cc)
target_link_libraries(main ${CONAN_LIBS})
