# this is hacky, and means you cannot distribute the executable;
# could be wrapped with a DEBUG conditional so that it switches to 
# relative dir when release.
add_compile_definitions(ROOT_DIR="${CMAKE_SOURCE_DIR}")
add_compile_definitions(RESOURCE_DIR="${CMAKE_SOURCE_DIR}/res")
add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/res/shaders")


add_executable(main
  main.cc shader.cc ik.cc)

target_link_libraries(main ${CONAN_LIBS})

target_include_directories(main PUBLIC 
  ${inversekin_SOURCE_DIR}/include)

set_target_properties(main
  PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY "${inversekin_SOURCE_DIR}/bin/"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${inversekin_SOURCE_DIR}/bin/Debug"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${inversekin_SOURCE_DIR}/bin/Release"
)
