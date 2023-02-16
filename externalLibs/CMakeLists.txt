cmake_minimum_required(VERSION 3.14)
add_library(
        imgui STATIC 
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_glfw.h
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_opengl3.h
        imgui/backends/imgui_impl_opengl3_loader.h
)
target_include_directories(imgui PUBLIC imgui glfw/inc imgui/backends)

add_library(
        pugixml STATIC
        pugixml pugixml/src/pugixml.cpp
)

target_include_directories(pugixml PUBLIC pugixml/src)
