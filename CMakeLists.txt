cmake_minimum_required(VERSION 3.20)
project(fox_engine LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(fox_engine
    src/main.c
    src/lib/utils.c
    src/lib/shader.c
    src/lib/stb_image.c
    external/glad/src/glad.c
)

target_include_directories(fox_engine PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/cglm/include
)

target_link_libraries(fox_engine PRIVATE
    "${CMAKE_SOURCE_DIR}/external/glfw/lib-mingw-w64/libglfw3.a"
    opengl32
    gdi32
    user32
    shell32
)