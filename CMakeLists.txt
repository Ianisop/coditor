cmake_minimum_required(VERSION 3.16)

project(coditor LANGUAGES CXX C)

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

# --------------------------
# Tree-sitter runtime (choose ONE way)
# --------------------------

# Option A: Use the single-file runtime (simplest)
set(TS_RUNTIME
    src/libs/tree-sitter/lib/src/lib.c
)

# Option B: Use split files (if you really need fine-grained control)
# file(GLOB TS_RUNTIME "tree-sitter/lib/src/*.c")

# --------------------------
# C++ grammar
# --------------------------
set(TS_CPP
    src/libs/tree-sitter-cpp/src/parser.c
    src/libs/tree-sitter-cpp/src/scanner.c
)

# Ensure parser.c and scanner.c are compiled as C
set_source_files_properties(${TS_CPP} PROPERTIES LANGUAGE C)
set_source_files_properties(${TS_RUNTIME} PROPERTIES LANGUAGE C)

# --------------------------
# Executable
# --------------------------
add_executable(coditor
    ${TS_RUNTIME}
    ${TS_CPP}
    src/code_editor.cpp
    src/coditor.cpp
    src/file_explorer.cpp
    src/lsp_client.cpp
    ${IMGUI_SOURCES}
)


# --------------------------
# Includes
# --------------------------
target_include_directories(coditor PRIVATE
    src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    src/libs/glfw/include
    src/libs/tree-sitter/lib/include      
    src/libs/tree-sitter/lib/src
    src/libs/tree-sitter-cpp/src
)

# --------------------------
# Linking
# --------------------------
find_package(OpenGL REQUIRED)

target_link_libraries(coditor PRIVATE
    glfw
    OpenGL::GL
)
