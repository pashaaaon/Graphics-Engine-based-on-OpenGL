// Main engine file
// https://github.com/pashaaaon/Graphics-Engine-based-on-OpenGL

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <filesystem>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

std::string SYSTEM_current_path = std::filesystem::current_path().string();
#include "Loaders/obj_load.hpp"
#include "Loaders/shader_load.hpp"
#include "Loaders/RenderBuffers_load.hpp"

const char EAPI_version[] = "Engine v1.0-beta";

bool SYSTEM_Init = false;
bool SYSTEM_MouseFixed = false;

GLFWwindow *EAPI_MainWindow;

void* SYSTEM_current_Scene3D = nullptr;
void* SYSTEM_current_Scene2D = nullptr;
void* SYSTEM_current_SceneGUI = nullptr;

glm::vec3 SYSTEM_camera_Position = {0.0f, 0.0f, 0.0f};
glm::vec3 SYSTEM_camera_LookAt = {0.0f, 1.0f, 0.0f};
glm::vec3 SYSTEM_clear_color = {28.0f, 28.0f, 30.0f};

// ---------------------------------------------------------------------- Main Functions

bool EAPI_Init(bool Shaders_InfoLog = false, bool ScreenBuffer_Filtering = true) {
    if (SYSTEM_Init) {return false;}

    try {
        // OpenGL Init

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        EAPI_MainWindow = glfwCreateWindow(640, 480, EAPI_version, NULL, NULL);
        glfwMakeContextCurrent(EAPI_MainWindow);

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        glClearColor(SYSTEM_clear_color.r/255.0f, SYSTEM_clear_color.g/255.0f, SYSTEM_clear_color.b/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Shaders

        if (Shaders_InfoLog) {
            SYSTEM_SHADERS_3D_LOAD(true);
            SYSTEM_SHADERS_PICKING_LOAD(true);
            SYSTEM_SHADERS_POSTEFFECT_LOAD(true);
        }

        else {
            SYSTEM_SHADERS_3D_LOAD(false);
            SYSTEM_SHADERS_PICKING_LOAD(false);
            SYSTEM_SHADERS_POSTEFFECT_LOAD(false);
        }

        // Framebuffer

        SYSTEM_PostEffect_INIT(ScreenBuffer_Filtering);
        SYSTEM_Picking_INIT();

        // ...

        stbi_set_flip_vertically_on_load(true);

        glfwPollEvents();
        
    }

    catch (...) {return false;}

    SYSTEM_Init = true;
    return true;
}

void EAPI_UpdateEvents() {
    glfwPollEvents();
}

void EAPI_DestroyWindow() {
    glfwDestroyWindow(EAPI_MainWindow);
    glfwTerminate();
};

int EAPI_WindowIsClosed() {return glfwWindowShouldClose(EAPI_MainWindow);}

void EAPI_GetWindowSize(int *width, int *height) {glfwGetWindowSize(EAPI_MainWindow, width, height);}

void EAPI_SetWindowSize(int width, int height) {glfwSetWindowSize(EAPI_MainWindow, width, height);}

void EAPI_SetWindowName(const char *title) {glfwSetWindowTitle(EAPI_MainWindow, title);}

void EAPI_SetWindowIcon(const char *file_dir) {
    GLFWimage icon[1];
    icon[0].pixels = stbi_load(file_dir, &icon[0].width, &icon[0].height, nullptr, 4);

    glfwSetWindowIcon(EAPI_MainWindow, 1, icon);
}

void EAPI_GetVersion(const char *version[32]) {*version = EAPI_version;}

// ---------------------------------------------------------------------- Render Functions

#include "Render/Graphics3D.hpp"
#include "Render/Graphics2D.hpp"
#include "Render/GraphicsGUI.hpp"

#include "Render/Render.hpp"
#include "Logic/Logic.hpp"

// ---------------------------------------------------------------------- Engine Functions

void EAPI_GetCameraPosition(float *x, float *y, float *z) {
    if (x) {*x = SYSTEM_camera_Position.x;}
    if (y) {*y = SYSTEM_camera_Position.y;}
    if (z) {*z = SYSTEM_camera_Position.z;}
}

void EAPI_SetCameraPosition(float x, float y, float z) {
    SYSTEM_camera_Position.x = x;
    SYSTEM_camera_Position.y = y;
    SYSTEM_camera_Position.z = z;
}

void EAPI_GetCameraDirection(float *x, float *y, float *z) {
    if (x) {*x = SYSTEM_camera_LookAt.x;}
    if (y) {*y = SYSTEM_camera_LookAt.y;}
    if (z) {*z = SYSTEM_camera_LookAt.z;}
}

void EAPI_SetCameraDirection(float x, float y, float z) {
    using namespace glm;
    
    vec3 camera_dir;
    camera_dir.x = x;
    camera_dir.y = y;
    camera_dir.z = z;
    
    if (length(camera_dir) >= 0.000001f) {
        SYSTEM_camera_LookAt = normalize(camera_dir);
    }
}

void EAPI_GetCameraAngle(float *yaw, float *pitch) {
    using namespace glm;

    if (yaw) {*yaw = degrees(atan(SYSTEM_camera_LookAt.y, SYSTEM_camera_LookAt.x));}
    if (pitch) {*pitch = degrees(asin(SYSTEM_camera_LookAt.z));}
}

void EAPI_SetCameraAngle(float yaw, float pitch) {
    using namespace glm;
    
    vec3 camera_dir;
    camera_dir.x = cos(radians(yaw)) * cos(radians(pitch));
    camera_dir.z = sin(radians(pitch));
    camera_dir.y = sin(radians(yaw)) * cos(radians(pitch));
    
    SYSTEM_camera_LookAt = normalize(camera_dir);
}

void EAPI_CameraMoveToDirection(float x, float y) {
    using namespace glm;
    SYSTEM_camera_Position += x * normalize(cross(SYSTEM_camera_LookAt, vec3(0.0f, 0.0f, 1.0f)));
    SYSTEM_camera_Position += y * SYSTEM_camera_LookAt;
}

bool EAPI_GetKey(int ASCII_Key_Number) {
    if (glfwGetKey(EAPI_MainWindow, ASCII_Key_Number) == GLFW_PRESS) {return true;}
    return false;
}

void EAPI_GetMousePosition(float *x, float *y) {
    double pos_x, pos_y;
    glfwGetCursorPos(EAPI_MainWindow, &pos_x, &pos_y);

    if (x) {*x = pos_x;}
    if (y) {*y = pos_y;}
}

void EAPI_MouseLock(bool mode = true) {
    SYSTEM_MouseFixed = mode;
    if (mode) {glfwSetInputMode(EAPI_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);}
    else {glfwSetInputMode(EAPI_MainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);}
}

void EAPI_ClearColor(unsigned short Red, unsigned short Green, unsigned short Blue) {
    SYSTEM_clear_color = {Red, Green, Blue};
    glClearColor(static_cast<float>(Red)/255.0f, static_cast<float>(Green)/255.0f, static_cast<float>(Blue)/255.0f, 1.0f);
}