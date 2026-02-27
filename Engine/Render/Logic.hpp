// ------------------------------------------------ Select Logic

unsigned int SYSTEM_OBJECT_SELECT(unsigned short type = 0) {
    int current_WinSizeX, current_WinSizeY;
    glfwGetWindowSize(EAPI_MainWindow, &current_WinSizeX, &current_WinSizeY);

    double mouse_x, mouse_y;
    glfwGetCursorPos(EAPI_MainWindow, &mouse_x, &mouse_y);
    mouse_x = static_cast<int>(mouse_x);
    mouse_y = static_cast<int>(mouse_y);

    int pixelX = SYSTEM_LastBufferSize_X / 2;
    int pixelY = SYSTEM_LastBufferSize_Y / 2;
    if (!SYSTEM_MouseFixed) {
        pixelX = mouse_x * SYSTEM_LastBufferSize_X / current_WinSizeX;
        pixelY = mouse_y * SYSTEM_LastBufferSize_Y / current_WinSizeY;
    }

    bool check_X = true;
    bool check_Y = true;

    if (pixelX < 0 || pixelX >= SYSTEM_LastBufferSize_X) {check_X = false;}
    if (pixelY < 0 || pixelY >= SYSTEM_LastBufferSize_Y) {check_Y = false;}

    if (check_X && check_Y) {
        glBindFramebuffer(GL_FRAMEBUFFER, SYSTEM_Picking_FrameBuffer);
        switch (type) {
            case 0:
                glReadBuffer(GL_COLOR_ATTACHMENT0);
                break;
            case 1:
                glReadBuffer(GL_COLOR_ATTACHMENT1);
                break;
            case 2:
                glReadBuffer(GL_COLOR_ATTACHMENT2);
                break;
            default:
                return 0;
        }
        
        GLuint pixel;
        glReadPixels(pixelX, SYSTEM_LastBufferSize_Y - 1 - pixelY, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
        return pixel;
    }
    return 0;
}

EAPI_Object_3D *EAPI_SelectedMouseObject_3D() {
    return static_cast<EAPI_Scene_3D*>(SYSTEM_current_Scene3D)->SYSTEM_objects[SYSTEM_OBJECT_SELECT(0)];
}

unsigned int EAPI_SelectedMouseObject_2D() {
//     return static_cast<EAPI_Scene_2D*>(SYSTEM_current_Scene2D)->SYSTEM_objects[SYSTEM_OBJECT_SELECT(1)];
}

unsigned int EAPI_SelectedMouseObject_GUI() {
//     return static_cast<EAPI_Scene_GUI*>(SYSTEM_current_SceneGUI)->SYSTEM_objects[SYSTEM_OBJECT_SELECT(2)];
}

// ------------------------------------------------ Collision Logic

void EAPI_Collision3D(
                    EAPI_Object_3D *MainObject, EAPI_Object_3D *SecondObject, 
                    bool *x_positive, bool *x_negative, 
                    bool *y_positive, bool *y_negative,
                    bool *z_positive, bool *z_negative
                    ) 
{
    if (x_positive) {*x_positive = false;} if (x_negative) {*x_negative = false;}
    if (y_positive) {*y_positive = false;} if (y_negative) {*y_negative = false;}
    if (z_positive) {*z_positive = false;} if (z_negative) {*z_negative = false;}

    EAPI_Model_3D *model_A = MainObject->get_model();
    EAPI_Model_3D *model_B = SecondObject->get_model();

    if (!model_A || !model_B) {return;}

    float A_SizeX = model_A->SYSTEM_MaxMinCoords[0] - model_A->SYSTEM_MaxMinCoords[1];
    float A_SizeY = model_A->SYSTEM_MaxMinCoords[2] - model_A->SYSTEM_MaxMinCoords[3];
    float A_SizeZ = model_A->SYSTEM_MaxMinCoords[4] - model_A->SYSTEM_MaxMinCoords[5];
    float A_VertexScale = 2.0f / std::max(std::max(A_SizeX, A_SizeY), A_SizeZ);

    float B_SizeX = model_B->SYSTEM_MaxMinCoords[0] - model_B->SYSTEM_MaxMinCoords[1];
    float B_SizeY = model_B->SYSTEM_MaxMinCoords[2] - model_B->SYSTEM_MaxMinCoords[3];
    float B_SizeZ = model_B->SYSTEM_MaxMinCoords[4] - model_B->SYSTEM_MaxMinCoords[5];
    float B_VertexScale = 2.0f / std::max(std::max(B_SizeX, B_SizeY), B_SizeZ);

    float A_CenterX = (model_A->SYSTEM_MaxMinCoords[0] + model_A->SYSTEM_MaxMinCoords[1]) / 2;
    float A_CenterY = (model_A->SYSTEM_MaxMinCoords[2] + model_A->SYSTEM_MaxMinCoords[3]) / 2;
    float A_CenterZ = (model_A->SYSTEM_MaxMinCoords[4] + model_A->SYSTEM_MaxMinCoords[5]) / 2;
    
    float B_CenterX = (model_B->SYSTEM_MaxMinCoords[0] + model_B->SYSTEM_MaxMinCoords[1]) / 2;
    float B_CenterY = (model_B->SYSTEM_MaxMinCoords[2] + model_B->SYSTEM_MaxMinCoords[3]) / 2;
    float B_CenterZ = (model_B->SYSTEM_MaxMinCoords[4] + model_B->SYSTEM_MaxMinCoords[5]) / 2;

    glm::vec3 A_max = glm::vec3((model_A->SYSTEM_MaxMinCoords[0] - A_CenterX) * MainObject->scale_x * A_VertexScale + MainObject->position_x, 
                                (model_A->SYSTEM_MaxMinCoords[2] - A_CenterY) * MainObject->scale_y * A_VertexScale + MainObject->position_y, 
                                (model_A->SYSTEM_MaxMinCoords[4] - A_CenterZ) * MainObject->scale_z * A_VertexScale + MainObject->position_z);
    glm::vec3 A_min = glm::vec3((model_A->SYSTEM_MaxMinCoords[1] - A_CenterX) * MainObject->scale_x * A_VertexScale + MainObject->position_x, 
                                (model_A->SYSTEM_MaxMinCoords[3] - A_CenterY) * MainObject->scale_y * A_VertexScale + MainObject->position_y, 
                                (model_A->SYSTEM_MaxMinCoords[5] - A_CenterZ) * MainObject->scale_z * A_VertexScale + MainObject->position_z);

    glm::vec3 B_max = glm::vec3((model_B->SYSTEM_MaxMinCoords[0] - B_CenterX) * SecondObject->scale_x * B_VertexScale + SecondObject->position_x, 
                                (model_B->SYSTEM_MaxMinCoords[2] - B_CenterY) * SecondObject->scale_y * B_VertexScale + SecondObject->position_y, 
                                (model_B->SYSTEM_MaxMinCoords[4] - B_CenterZ) * SecondObject->scale_z * B_VertexScale + SecondObject->position_z);
    glm::vec3 B_min = glm::vec3((model_B->SYSTEM_MaxMinCoords[1] - B_CenterX) * SecondObject->scale_x * B_VertexScale + SecondObject->position_x, 
                                (model_B->SYSTEM_MaxMinCoords[3] - B_CenterY) * SecondObject->scale_y * B_VertexScale + SecondObject->position_y, 
                                (model_B->SYSTEM_MaxMinCoords[5] - B_CenterZ) * SecondObject->scale_z * B_VertexScale + SecondObject->position_z);

    float OverlapX = max(min(A_max.x, B_max.x) - max(A_min.x, B_min.x), 0.0f);
    float OverlapY = max(min(A_max.y, B_max.y) - max(A_min.y, B_min.y), 0.0f);
    float OverlapZ = max(min(A_max.z, B_max.z) - max(A_min.z, B_min.z), 0.0f);

    if (MainObject->SYSTEM_collisionType == 0 && SecondObject->SYSTEM_collisionType == 0) {
        // X_positive (right)
        if (OverlapX > 0.0f && OverlapX > OverlapY && OverlapX > OverlapZ && \
            A_CenterX < B_CenterX && A_max.x >= B_min.x) {*x_positive = true;}

        // X_negative (left)
        if (OverlapX > 0.0f && OverlapX > OverlapY && OverlapX > OverlapZ && \
            A_CenterX > B_CenterY && A_min.x <= B_max.x) {*x_positive = true;}

        // Y_positive (front)

        // Y_negative (back)

        // Z_positive (up)

        // Z_negative (down)
    }
}