cmake_minimum_required(VERSION 3.15)
project(Engine-v1.0)

add_library(glad ${PROJECT_SOURCE_DIR}/External/glad/src/glad.c)
target_include_directories(glad PRIVATE ${PROJECT_SOURCE_DIR}/External/glad/include)

add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)

target_include_directories(main PRIVATE 
    ${PROJECT_SOURCE_DIR}/External/glad/include 
    ${PROJECT_SOURCE_DIR}/External/glfw-3.4/include 
    ${PROJECT_SOURCE_DIR}/External/glm-1.0.3/glm 
    ${PROJECT_SOURCE_DIR}/External/stb_image
)

target_link_libraries(main 
    opengl32 
    glad 
    ${PROJECT_SOURCE_DIR}/External/glfw-3.4/lib-mingw-w64/libglfw3.a
)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Compiled_Release)
file(COPY ${PROJECT_SOURCE_DIR}/Shaders DESTINATION ${PROJECT_SOURCE_DIR}/Compiled_Release)
file(COPY ${PROJECT_SOURCE_DIR}/Content DESTINATION ${PROJECT_SOURCE_DIR}/Compiled_Release)