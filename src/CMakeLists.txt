include_directories(${inversekin_SOURCE_DIR}/include)

# this is hacky, and means you cannot distribute the executable
# could be wrapped with a DEBUG conditional so that it switches to 
# relative dir when release.
add_compile_definitions(ROOT_DIR="${CMAKE_SOURCE_DIR}")
add_compile_definitions(RESOURCE_DIR="${CMAKE_SOURCE_DIR}/res")
add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/res/shaders")

add_executable(main main.cc shader.cc)
target_link_libraries(main ${CONAN_LIBS})
