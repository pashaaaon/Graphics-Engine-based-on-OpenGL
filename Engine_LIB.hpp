// Main file
// https://github.com/pasha-oni/Graphics-Engine-based-on-OpenGL

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

std::string current_path = std::filesystem::current_path().string();
#include "Loaders/obj_load.hpp"
#include "Loaders/shader_load.hpp"
#include "Loaders/RenderBuffers_load.hpp"

const char EAPI_version[] = "Engine v1.0-beta";

bool Init = false;
GLFWwindow *EAPI_MainWindow;

void* current_Scene3D = nullptr;
void* current_Scene2D = nullptr;
void* current_SceneGUI = nullptr;

glm::vec3 camera_Position = {0.0f, 0.0f, 0.0f};
glm::vec3 camera_LookAt = {0.0f, 1.0f, 0.0f};

// ---------- System Functions

bool EAPI_Init(bool Shaders_InfoLog = false, bool ScreenBuffer_Linear = true) {
    if (Init) {return false;}

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

        glClearColor(28.0f/255.0f, 28.0f/255.0f, 30.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Shaders

        if (Shaders_InfoLog) {
            SYSTEM_SHADERS_3D_LOAD(true);
            SYSTEM_SHADERS_POSTEFFECT_LOAD(true);
        }

        else {
            SYSTEM_SHADERS_3D_LOAD(false);
            SYSTEM_SHADERS_POSTEFFECT_LOAD(false);
        }

        // Framebuffer

        SYSTEM_PostEffect_INIT(ScreenBuffer_Linear);

        // ...

        glfwPollEvents();
    }

    catch (...) {return false;}

    Init = true;
    return true;
}

void EAPI_UpdateWindow() {
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

void EAPI_GetVersion(const char *version[32]) {*version = EAPI_version;}

// ---------- Engine Functions

void EAPI_GetCameraPosition(float *x, float *y, float *z) {
    if (x) {*x = camera_Position.x;}
    if (y) {*y = camera_Position.y;}
    if (z) {*z = camera_Position.z;}
}

void EAPI_SetCameraPosition(float x, float y, float z) {
    camera_Position.x = x;
    camera_Position.y = y;
    camera_Position.z = z;
}

void EAPI_GetCameraDirection(float *x, float *y, float *z) {
    if (x) {*x = camera_LookAt.x;}
    if (y) {*y = camera_LookAt.y;}
    if (z) {*z = camera_LookAt.z;}
}

void EAPI_SetCameraDirection(float x, float y, float z) {
    using namespace glm;
    
    vec3 camera_dir;
    camera_dir.x = x;
    camera_dir.y = y;
    camera_dir.z = z;
    
    if (length(camera_dir) >= 0.000001f) {
        camera_LookAt = normalize(camera_dir);
    }
}

void EAPI_GetCameraAngle(float *yaw, float *pitch) {
    using namespace glm;

    if (yaw) {*yaw = degrees(atan(camera_LookAt.y, camera_LookAt.x));}
    if (pitch) {*pitch = degrees(asin(camera_LookAt.z));}
}

void EAPI_SetCameraAngle(float yaw, float pitch) {
    using namespace glm;
    
    vec3 camera_dir;
    camera_dir.x = cos(radians(yaw)) * cos(radians(pitch));
    camera_dir.z = sin(radians(pitch));
    camera_dir.y = sin(radians(yaw)) * cos(radians(pitch));
    
    camera_LookAt = normalize(camera_dir);
}

void EAPI_CameraMoveToDirection(float x, float y) {
    using namespace glm;
    camera_Position += x * normalize(cross(camera_LookAt, vec3(0.0f, 0.0f, 1.0f)));
    camera_Position += y * camera_LookAt;
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
    glfwSetInputMode(EAPI_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void EAPI_ClearColor(float Red, float Green, float Blue) {
    glClearColor(Red/255.0f, Green/255.0f, Blue/255.0f, 1.0f);
}

// ---------- Render Functions

#include "Render/Graphics3D.hpp"
#include "Render/Graphics2D.hpp"
#include "Render/GraphicsGUI.hpp"

#include "Render/Render.hpp"