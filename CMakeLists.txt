cmake_minimum_required(VERSION 3.20)
project(opengl_school LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(opengl_school
    main.c
    external/glad/src/glad.c
)

target_include_directories(opengl_school PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_link_libraries(opengl_school PRIVATE
    "${CMAKE_SOURCE_DIR}/external/glfw/lib-mingw-w64/libglfw3.a"
    opengl32
    gdi32
    user32
    shell32
)