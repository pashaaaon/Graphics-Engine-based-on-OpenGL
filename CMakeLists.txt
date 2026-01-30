cmake_minimum_required(VERSION 3.15)
project(Engine-v1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(Engine_LIB ${PROJECT_SOURCE_DIR}/Engine_LIB.cpp)
add_library(glad ${PROJECT_SOURCE_DIR}/External/glad/src/glad.c)

target_include_directories(glad PRIVATE ${PROJECT_SOURCE_DIR}/External/glad/include)
target_include_directories(Engine_LIB PRIVATE ${PROJECT_SOURCE_DIR}/External/glad/include ${PROJECT_SOURCE_DIR}/External/glfw-3.4/include ${PROJECT_SOURCE_DIR}/External/glm-1.0.3/glm ${PROJECT_SOURCE_DIR}/External/stb_image)

target_link_libraries(Engine_LIB opengl32 glad ${PROJECT_SOURCE_DIR}/External/glfw-3.4/lib-mingw-w64/libglfw3.a)