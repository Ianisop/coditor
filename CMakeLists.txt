cmake_minimum_required(VERSION 3.16)

project(coditor LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --------------------------
# GLFW (from src/libs/glfw)
# --------------------------
add_subdirectory(src/libs/glfw)

# --------------------------
# ImGui (from src/libs/imgui)
# --------------------------
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/src/libs/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


add_executable(coditor
    src/coditor.cpp
    src/coditor.h
    src/file_explorer.cpp
    src/file_explorer.h
    ${IMGUI_SOURCES}
)

# --------------------------
# Includes + Linking
# --------------------------
find_package(OpenGL REQUIRED)

target_include_directories(coditor PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    src/libs/glfw/include
)

target_link_libraries(coditor PRIVATE
    glfw
    OpenGL::GL
)
