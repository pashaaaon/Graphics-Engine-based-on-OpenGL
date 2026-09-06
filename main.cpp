#include "Engine/Engine_LIB.hpp"

int main() {
    // ------------------------------------------------------- Init the engine
    EAPI_Init(true, true);

    // ------------------------------------------------------- Load models and map setting

    EAPI_Model_3D *terrain_model = new EAPI_Model_3D("Content/Test Models/Other/flat.obj");
    EAPI_Model_3D *test_model1 = new EAPI_Model_3D("Content//Test Models/Other/cube.obj");

    EAPI_Object_3D *test_terrain = new EAPI_Object_3D(terrain_model);
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
        camera_entity_object->scale_x = 25.0f;
        camera_entity_object->scale_y = 25.0f;
        camera_entity_object->scale_z = 25.0f;

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
        // The camera entity is not added to the scene because it does not need to be rendered. The object is created solely for collision checking with the cube.

    EAPI_SetCameraPosition(0.0f, 300.0f, 300.0f);
    EAPI_MouseLock(true);
    EAPI_SelectScene3D(Scene);

    // ------------------------------------------------------- Main render loop
    while (!EAPI_WindowIsClosed()) {

        // ------------------------------------------------------- Gets window size
        int win_width, win_height;
        EAPI_GetWindowSize(&win_width, &win_height);

        // ------------------------------------------------------- Camera rotation
        float mouse_x, mouse_y;
        EAPI_GetMousePosition(&mouse_x, &mouse_y);

        float yaw = -mouse_x/12.0f;
        float pitch = -mouse_y/12.0f;
        EAPI_SetCameraAngle(yaw, pitch);

        // ------------------------------------------------------- Movement and collision
        float dx = 0.0f;
        float dy = 0.0f;
        float dz = 0.0f;

        float cam_x, cam_y, cam_z;
        EAPI_GetCameraPosition(&cam_x, &cam_y, &cam_z);

        if (EAPI_GetKey(GLFW_KEY_W)) {dy += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_S)) {dy -= 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_A)) {dx -= 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_D)) {dx += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_SPACE)) {dz += 0.5f;}
        if (EAPI_GetKey(GLFW_KEY_LEFT_SHIFT)) {dz -= 0.5f;}

        EAPI_CameraMoveToDirection(dx, dy);

        float new_cam_x, new_cam_y, new_cam_z;
        EAPI_GetCameraPosition(&new_cam_x, &new_cam_y, &new_cam_z);
        new_cam_z += dz;

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

        // ------------------------------------------------------- Mouse object picking
        EAPI_Object_3D *Selected_Object = EAPI_SelectedMouseObject_3D();

        if (Selected_Object != nullptr) {
            if (Selected_Object->SYSTEM_index_in_scene == 1) {cout << "You are looking at: Terrain" << endl;}
            else if (Selected_Object->SYSTEM_index_in_scene == 2) {cout << "You are looking at: Cube" << endl;}
            else {cout << "You are looking at (object index): " << Selected_Object->SYSTEM_index_in_scene << endl;}
        }
        else {cout << "You are looking at: None" << endl;}

        // ------------------------------------------------------- Render and update the window
        EAPI_Render(win_width, win_height);
        EAPI_UpdateEvents();
        
    }

    return 0;
}
