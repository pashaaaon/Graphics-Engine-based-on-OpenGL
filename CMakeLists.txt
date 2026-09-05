cmake_minimum_required(VERSION 3.15)
project(Engine-v1.0)

add_executable(main WIN32 ${PROJECT_SOURCE_DIR}/main.cpp)

target_include_directories(main PRIVATE 
    ${PROJECT_SOURCE_DIR}/Engine/External/glad/include 
    ${PROJECT_SOURCE_DIR}/Engine/External/glfw-3.4/include 
    ${PROJECT_SOURCE_DIR}/Engine/External/glm-1.0.3/glm 
    ${PROJECT_SOURCE_DIR}/Engine/External/stb_image
)

target_link_libraries(main 
    opengl32 
    ${PROJECT_SOURCE_DIR}/Engine/External/glad/src/libglad.a 
    ${PROJECT_SOURCE_DIR}/Engine/External/glfw-3.4/lib-mingw-w64/libglfw3.a
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Debug-Release)

add_custom_target(CopyResources ALL 
    COMMAND robocopy "${PROJECT_SOURCE_DIR}/Engine/Shaders" "${PROJECT_SOURCE_DIR}/Debug-Release/Shaders" /MIR >nul & exit 0
    COMMAND robocopy "${PROJECT_SOURCE_DIR}/Content" "${PROJECT_SOURCE_DIR}/Debug-Release/Content" /MIR >nul & exit 0
)
