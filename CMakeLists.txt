cmake_minimum_required(VERSION 3.14)

project(APIS3D_2023 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG> CACHE STRING "")


set(ASSIMP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/externalLibs/assimp/include)
find_library(ASSIMP_LIBRARY NAMES assimp-vc142-mtd PATHS ${CMAKE_SOURCE_DIR}/externalLibs/assimp/lib/Debug)

add_subdirectory("externalLibs")

link_directories(
    ${CMAKE_SOURCE_DIR}/externalLibs/glfw/lib
)

add_executable(APIS3D 
    src/APIS3D.cpp
    src/asian_town.cpp
    src/billboard.cpp
    src/camera.cpp
    src/camera_keyboard.cpp
    src/column.cpp
    src/cube_light.cpp 
    src/cube_text.cpp
    src/directional_light.cpp 
    src/emitter.cpp
    src/entity.cpp
    src/factory_engine.cpp
    src/imgui_app.cpp
    src/flash_light.cpp
    src/gl1_render.cpp
    src/gl4_render.cpp
    src/gl_texture.cpp
    src/glfw_input_manager.cpp
    src/glsl_material.cpp
    src/glsl_shader.cpp
    src/light.cpp
    src/material.cpp
    src/mesh3d.cpp
    src/particle.cpp
    src/point_light.cpp
    src/object.cpp
    src/object3d.cpp
    src/render.cpp
    src/rotate_camera.cpp
    src/system.cpp
    src/texture.cpp
    src/time_manager.cpp
    src/triangle_rot.cpp
    src/orbital_light.cpp
  )

target_include_directories(
    APIS3D 
    PRIVATE 
    src
    ${CMAKE_SOURCE_DIR}/externalLibs/glad/inc
    ${CMAKE_SOURCE_DIR}/externalLibs/glfw/inc
    ${CMAKE_SOURCE_DIR}/externalLibs/glm/inc
    ${CMAKE_SOURCE_DIR}/externalLibs/stb
    ${CMAKE_SOURCE_DIR}/externalLibs/assimp/include
)

target_link_libraries(
  APIS3D 
  PRIVATE
  glfw3
  imgui
  pugixml
  ${ASSIMP_LIBRARY}
)

add_custom_command(TARGET APIS3D POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data.
)
add_custom_command(TARGET APIS3D POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/assimp/bin/Debug/assimp-vc142-mtd.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)