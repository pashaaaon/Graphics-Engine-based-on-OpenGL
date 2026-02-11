#include "Engine_LIB.hpp"

int main() {
    EAPI_Init(true, true);

    EAPI_Model_3D *test_model = new EAPI_Model_3D("Content/Test Models/Other/UVsphere.obj");
    // test_model.texture_pixeled(true);
    
    EAPI_Object_3D *test_object1 = new EAPI_Object_3D(test_model);
    test_object1->scale_x = 50.0f;
    test_object1->scale_y = 50.0f;
    test_object1->scale_z = 50.0f;
    test_object1->rotate_angle_x = 90.0f;

    EAPI_Light_3D *test_light1 = new EAPI_Light_3D;
    test_light1->SetLightType_point();
    test_light1->position_x = 50.0f;
    test_light1->position_y = 50.0f;
    test_light1->position_z = 50.0f;
    test_light1->strength = 2.0f;
    test_light1->set_direction(0.0f, 0.0f, 0.0f);

    EAPI_Scene_3D *Scene = new EAPI_Scene_3D;
    Scene->add_object(test_object1);
    Scene->add_light(test_light1);

    // EAPI_MouseLock(true);
    EAPI_SelectScene3D(Scene);

    while (!EAPI_WindowIsClosed()) {
        int win_width, win_height;
        EAPI_GetWindowSize(&win_width, &win_height);

        float mouse_x, mouse_y;
        EAPI_GetMousePosition(&mouse_x, &mouse_y);

        float cam_x, cam_y, cam_z;
        EAPI_GetCameraPosition(&cam_x, &cam_y, &cam_z);

        float x = 0;
        float y = 0;

        if (EAPI_GetKey(GLFW_KEY_W)) {y += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_S)) {y -= 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_A)) {x -= 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_D)) {x += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_SPACE)) {cam_z += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_LEFT_SHIFT)) {cam_z -= 0.5f;}

        float yaw = -mouse_x/12.0f;
        float pitch = -mouse_y/12.0f;

        EAPI_SetCameraPosition(cam_x, cam_y, cam_z);
        EAPI_SetCameraAngle(yaw, pitch);
        EAPI_CameraMoveToDirection(x, y);

        EAPI_Render(win_width*2, win_height*2);
        EAPI_UpdateWindow();

        cout << "object3D:" << EAPI_SelectedMouseObject_3D() << endl;
        // cout << glGetError();
    }
    
    return 0;
}