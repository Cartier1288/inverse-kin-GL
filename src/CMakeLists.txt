add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${inversekin_SOURCE_DIR}/include)

add_executable(main main.cc)
target_link_libraries(main ${CONAN_LIBS})
