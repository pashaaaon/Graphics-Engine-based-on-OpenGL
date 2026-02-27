#include "Engine/Engine_LIB.hpp"

int main() {
    EAPI_Init(true, true);

    EAPI_Model_3D *terrain_model = new EAPI_Model_3D("Content/Test Models/Other/flat.obj");
    EAPI_Model_3D *test_model1 = new EAPI_Model_3D("Content/Test Models/Other/UVsphere.obj");

    EAPI_Object_3D * test_terrain = new EAPI_Object_3D(terrain_model);
        test_terrain->scale_x = 500.0f;
        test_terrain->scale_y = 500.0f;
        test_terrain->scale_z = 500.0f;
        test_terrain->rotate_angle_x = 90.0f;

    EAPI_Object_3D *test_object1 = new EAPI_Object_3D(test_model1);
        test_object1->position_x = 50.0f;
        test_object1->position_z = 100.0f;
        test_object1->scale_x = 100.0f;
        test_object1->scale_y = 100.0f;
        test_object1->scale_z = 100.0f;
        test_object1->rotate_angle_x = 90.0f;

    // EAPI_Object_3D *camera_entity_object = new EAPI_Object_3D(test_model1);

    EAPI_Light_3D *test_light1 = new EAPI_Light_3D;
        test_light1->SetLightType_spot();
        test_light1->position_x = 0.0f;
        test_light1->position_y = 0.0f;
        test_light1->position_z = 500.0f;
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

        EAPI_Render(win_width * 8, win_height * 8);
        EAPI_UpdateEvents();

        // bool x_positive, x_negative, y_positive, y_negative, z_positive, z_negative;
        // EAPI_GetCameraPosition(&cam_x, &cam_y, &cam_z);
        // camera_entity_object->position_x = cam_x; camera_entity_object->position_y = cam_y; camera_entity_object->position_z = cam_z;
        // EAPI_Collision3D(test_object1, camera_entity_object, &x_positive, &x_negative, &y_positive, &y_negative, &z_positive, &z_negative);
        // cout << x_positive << x_negative << y_positive << y_negative << z_positive << z_negative << "   " << cam_x << ' ' << cam_y << ' ' << cam_z << endl;
    }
    
    return 0;
}