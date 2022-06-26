include_directories(${inversekin_SOURCE_DIR}/include)

add_executable(main main.cc shader.cc)
target_link_libraries(main ${CONAN_LIBS})
