#include "Engine/Engine_LIB.hpp"

int main() {
    EAPI_Init(true, true);

    EAPI_Model_3D *terrain_model = new EAPI_Model_3D("Content/Test Models/Other/flat.obj");
    EAPI_Model_3D *test_model1 = new EAPI_Model_3D("Content//Test Models/Other/cube.obj");

    EAPI_Object_3D * test_terrain = new EAPI_Object_3D(terrain_model);
        test_terrain->scale_x = 500.0f;
        test_terrain->scale_y = 500.0f;
        test_terrain->scale_z = 500.0f;
        test_terrain->rotate_angle_x = 90.0f;

    EAPI_Object_3D *test_object1 = new EAPI_Object_3D(test_model1);
        test_object1->position_x = 0.0f;
        test_object1->position_y = 0.0f;
        test_object1->position_z = 100.0f;
        test_object1->scale_x = 50.0f;
        test_object1->scale_y = 50.0f;
        test_object1->scale_z = 50.0f;
        test_object1->rotate_angle_x = 90.0f;

    EAPI_Object_3D *camera_entity_object = new EAPI_Object_3D(test_model1);

    EAPI_Light_3D *test_light1 = new EAPI_Light_3D;
        test_light1->SetLightType_spot();
        test_light1->position_x = -100.0f;
        test_light1->position_y = 100.0f;
        test_light1->position_z = 400.0f;
        test_light1->strength = 3.0f;
        test_light1->set_direction(0.0f, 0.0f, 0.0f);

    EAPI_Scene_3D *Scene = new EAPI_Scene_3D;
        Scene->add_object(test_terrain);
        Scene->add_object(test_object1);
        Scene->add_light(test_light1);

    EAPI_MouseLock(true);
    EAPI_SelectScene3D(Scene);

    while (!EAPI_WindowIsClosed()) {
        int win_width, win_height;
        EAPI_GetWindowSize(&win_width, &win_height);

        float mouse_x, mouse_y;
        EAPI_GetMousePosition(&mouse_x, &mouse_y);

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        float cam_x, cam_y, cam_z;
        EAPI_GetCameraPosition(&cam_x, &cam_y, &cam_z);

        if (EAPI_GetKey(GLFW_KEY_W)) {y += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_S)) {y -= 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_A)) {x -= 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_D)) {x += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_SPACE)) {z += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_LEFT_SHIFT)) {z -= 0.5f;}

        EAPI_CameraMoveToDirection(x, y);

        float new_cam_x, new_cam_y, new_cam_z;
        EAPI_GetCameraPosition(&new_cam_x, &new_cam_y, &new_cam_z);
        new_cam_z += z;

        bool x_positive, x_negative, y_positive, y_negative, z_positive, z_negative;
        camera_entity_object->position_x = new_cam_x; camera_entity_object->position_y = new_cam_y; camera_entity_object->position_z = new_cam_z;
        EAPI_Collision3D(test_object1, camera_entity_object, &x_positive, &x_negative, &y_positive, &y_negative, &z_positive, &z_negative);

        if (x_positive) {EAPI_SetCameraPosition(cam_x, new_cam_y, new_cam_z);}
        else if (x_negative) {EAPI_SetCameraPosition(cam_x, new_cam_y, new_cam_z);}
        else if (y_positive) {EAPI_SetCameraPosition(new_cam_x, cam_y, new_cam_z);}
        else if (y_negative) {EAPI_SetCameraPosition(new_cam_x, cam_y, new_cam_z);}
        else if (z_positive) {EAPI_SetCameraPosition(new_cam_x, new_cam_y, cam_z);}
        else if (z_negative) {EAPI_SetCameraPosition(new_cam_x, new_cam_y, cam_z);}
        else {EAPI_SetCameraPosition(new_cam_x, new_cam_y, new_cam_z);}

        float yaw = -mouse_x/12.0f;
        float pitch = -mouse_y/12.0f;
        EAPI_SetCameraAngle(yaw, pitch);

        EAPI_Render(win_width, win_height);
        EAPI_UpdateEvents();
    }
    
    return 0;
}