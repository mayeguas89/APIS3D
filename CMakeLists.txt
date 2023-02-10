cmake_minimum_required(VERSION 3.14)

project(APIS3D_2023 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG> CACHE STRING "")

add_subdirectory("externalLibs")

link_directories(
    ${CMAKE_SOURCE_DIR}/externalLibs/glfw/lib
)

add_executable(APIS3D 
    src/camera.cpp
    src/camera_keyboard.cpp
    src/cube_text.cpp
    src/entity.cpp
    src/factory_engine.cpp
    src/gl1_render.cpp
    src/gl4_render.cpp
    src/gl_texture.cpp
    src/glfw_input_manager.cpp
    src/glsl_material.cpp
    src/glsl_shader.cpp
    src/APIS3D.cpp
    src/material.cpp
    src/mesh3d.cpp
    src/object.cpp
    src/object3d.cpp
    src/render.cpp
    src/system.cpp
    src/texture.cpp
    src/time_manager.cpp
    src/triangle_rot.cpp
)

target_include_directories(
    APIS3D 
    PRIVATE 
    src
    ${CMAKE_SOURCE_DIR}/externalLibs/glad/inc
    ${CMAKE_SOURCE_DIR}/externalLibs/glfw/inc
    ${CMAKE_SOURCE_DIR}/externalLibs/glm/inc
    ${CMAKE_SOURCE_DIR}/externalLibs/stb
)

target_link_libraries(
  APIS3D 
  PRIVATE
  glfw3
  imgui
)

add_custom_command(TARGET APIS3D POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data.
)