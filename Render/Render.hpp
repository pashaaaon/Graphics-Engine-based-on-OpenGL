void EAPI_Render(unsigned short Custom_Buffer_Width = false, unsigned short Custom_Buffer_Height = false) {
    using namespace glm;

    glfwPollEvents();

    glBindFramebuffer(GL_FRAMEBUFFER, PostEffect_FrameBuffer);
    
    unsigned short WinSizeX = Custom_Buffer_Width;
    unsigned short WinSizeY = Custom_Buffer_Height;

    if (!Custom_Buffer_Width || !Custom_Buffer_Height) {
        int ScreenWinSizeX = 0;
        int ScreenWinSizeY = 0;
        glfwGetWindowSize(EAPI_MainWindow, &ScreenWinSizeX, &ScreenWinSizeY);
        if (!ScreenWinSizeX || !ScreenWinSizeY) {ScreenWinSizeX = 1; ScreenWinSizeY = 1;}

        if (!Custom_Buffer_Width) {WinSizeX = ScreenWinSizeX;}
        if (!Custom_Buffer_Height) {WinSizeY = ScreenWinSizeY;}
    }
    
    glViewport(0, 0, WinSizeX, WinSizeY);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WinSizeX, WinSizeY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WinSizeX, WinSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --------------------- Render 3D scene
    if (current_Scene3D) {
        static_cast<EAPI_Scene_3D*>(current_Scene3D)->update_light();

        mat4 Projection_Matrix = perspective(radians(45.0f), static_cast<float>(WinSizeX)/static_cast<float>(WinSizeY), 0.1f, 10000.0f);
        mat4 View_Matrix = lookAt(camera_Position, camera_Position + camera_LookAt, vec3(0.0f, 0.0f, 1.0f));
        mat4 ProjView_Matrix = Projection_Matrix * View_Matrix;

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_BUFFER, static_cast<EAPI_Scene_3D*>(current_Scene3D)->light_texture);

        glUseProgram(default_3D_shaders_illum0);
        glUniformMatrix4fv(default_3D_shaders_illum0_ProjView, 1, GL_FALSE, value_ptr(ProjView_Matrix));

        glUseProgram(default_3D_shaders_illum1);
        glUniformMatrix4fv(default_3D_shaders_illum1_ProjView, 1, GL_FALSE, value_ptr(ProjView_Matrix));
        glUniform3fv(default_3D_shaders_illum1_camDirection, 1, value_ptr(camera_LookAt));
        glUniform3fv(default_3D_shaders_illum1_camPosition, 1, value_ptr(camera_Position));
        glUniform1i(default_3D_shaders_illum1_LightBuffer, 1);
        glUniform1i(default_3D_shaders_illum1_LightBufferSize, static_cast<EAPI_Scene_3D*>(current_Scene3D)->light_buffer_size);

        glUseProgram(default_3D_shaders_illum2);
        glUniformMatrix4fv(default_3D_shaders_illum2_ProjView, 1, GL_FALSE, value_ptr(ProjView_Matrix));
        glUniform3fv(default_3D_shaders_illum2_camDirection, 1, value_ptr(camera_LookAt));
        glUniform3fv(default_3D_shaders_illum2_camPosition, 1, value_ptr(camera_Position));
        glUniform1i(default_3D_shaders_illum2_LightBuffer, 1);
        glUniform1i(default_3D_shaders_illum2_LightBufferSize, static_cast<EAPI_Scene_3D*>(current_Scene3D)->light_buffer_size);

        for (EAPI_Object_3D* &object : static_cast<EAPI_Scene_3D*>(current_Scene3D)->objects) {
            if (!object) {continue;}
            EAPI_Model_3D *Model_3D = object->get_model();
            if (!Model_3D || !Model_3D->model_available) {continue;}

            mat4 Model_Matrix = mat4(1.0f);
            Model_Matrix = translate(Model_Matrix, vec3(object->position_x, object->position_y, object->position_z));
            Model_Matrix = rotate(Model_Matrix, radians(object->rotate_angle_x), vec3(1.0f, 0.0f, 0.0f));
            Model_Matrix = rotate(Model_Matrix, radians(object->rotate_angle_y), vec3(0.0f, 1.0f, 0.0f));
            Model_Matrix = rotate(Model_Matrix, radians(object->rotate_angle_z), vec3(0.0f, 0.0f, 1.0f));
            Model_Matrix = scale(Model_Matrix, vec3(object->scale_x, object->scale_y, object->scale_z));
            
            glUseProgram(default_3D_shaders_illum0);
            glUniformMatrix4fv(default_3D_shaders_illum0_model, 1, GL_FALSE, value_ptr(Model_Matrix));
            glUniform1i(default_3D_shaders_illum0_TextureBool, Model_3D->TextureBool);

            glUseProgram(default_3D_shaders_illum1);
            glUniformMatrix4fv(default_3D_shaders_illum1_model, 1, GL_FALSE, value_ptr(Model_Matrix));
            glUniform1i(default_3D_shaders_illum1_TextureBool, Model_3D->TextureBool);

            glUseProgram(default_3D_shaders_illum2);
            glUniformMatrix4fv(default_3D_shaders_illum2_model, 1, GL_FALSE, value_ptr(Model_Matrix));
            glUniform1i(default_3D_shaders_illum2_TextureBool, Model_3D->TextureBool);
            
            for(int i = 0; i<Model_3D->VAOs.size(); i+=1) {
                if (!Model_3D->VBOs_Size[i]) {continue;}

                switch (Model_3D->Materials[i].illum) {
                    case 0:
                        glUseProgram(default_3D_shaders_illum0);
                        
                        glUniform1f(default_3D_shaders_illum0_d, Model_3D->Materials[i].d);
                        glUniform3fv(default_3D_shaders_illum0_Kd, 1, value_ptr(vec3(Model_3D->Materials[i].Kd[0], Model_3D->Materials[i].Kd[1], Model_3D->Materials[i].Kd[2])));

                        glActiveTexture(GL_TEXTURE2);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+1]);
                        glUniform1i(default_3D_shaders_illum0_mapKd, 2);
                        break;

                    case 1:
                        glUseProgram(default_3D_shaders_illum1);
                        
                        glUniform1f(default_3D_shaders_illum1_d, Model_3D->Materials[i].d);
                        glUniform3fv(default_3D_shaders_illum1_Ka, 1, value_ptr(vec3(Model_3D->Materials[i].Ka[0], Model_3D->Materials[i].Ka[1], Model_3D->Materials[i].Ka[2])));
                        glUniform3fv(default_3D_shaders_illum1_Kd, 1, value_ptr(vec3(Model_3D->Materials[i].Kd[0], Model_3D->Materials[i].Kd[1], Model_3D->Materials[i].Kd[2])));

                        glActiveTexture(GL_TEXTURE2);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+0]);
                        glUniform1i(default_3D_shaders_illum1_mapKa, 2);

                        glActiveTexture(GL_TEXTURE3);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+1]);
                        glUniform1i(default_3D_shaders_illum1_mapKd, 3);
                        break;

                    case 2:
                        glUseProgram(default_3D_shaders_illum2);
                        
                        glUniform1f(default_3D_shaders_illum2_d, Model_3D->Materials[i].d);
                        glUniform1f(default_3D_shaders_illum2_Ns, Model_3D->Materials[i].Ns);
                        glUniform3fv(default_3D_shaders_illum2_Ka, 1, value_ptr(vec3(Model_3D->Materials[i].Ka[0], Model_3D->Materials[i].Ka[1], Model_3D->Materials[i].Ka[2])));
                        glUniform3fv(default_3D_shaders_illum2_Kd, 1, value_ptr(vec3(Model_3D->Materials[i].Kd[0], Model_3D->Materials[i].Kd[1], Model_3D->Materials[i].Kd[2])));
                        glUniform3fv(default_3D_shaders_illum2_Ks, 1, value_ptr(vec3(Model_3D->Materials[i].Ks[0], Model_3D->Materials[i].Ks[1], Model_3D->Materials[i].Ks[2])));

                        glActiveTexture(GL_TEXTURE2);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+0]);
                        glUniform1i(default_3D_shaders_illum2_mapKa, 2);
                        
                        glActiveTexture(GL_TEXTURE3);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+1]);
                        glUniform1i(default_3D_shaders_illum2_mapKd, 3);

                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+2]);
                        glUniform1i(default_3D_shaders_illum2_mapKs, 4);
                        break;

                    default:
                        glUseProgram(default_3D_shaders_illum2);

                        glUniform1f(default_3D_shaders_illum2_d, Model_3D->Materials[i].d);
                        glUniform1f(default_3D_shaders_illum2_Ns, Model_3D->Materials[i].Ns);
                        glUniform3fv(default_3D_shaders_illum2_Ka, 1, value_ptr(vec3(Model_3D->Materials[i].Ka[0], Model_3D->Materials[i].Ka[1], Model_3D->Materials[i].Ka[2])));
                        glUniform3fv(default_3D_shaders_illum2_Kd, 1, value_ptr(vec3(Model_3D->Materials[i].Kd[0], Model_3D->Materials[i].Kd[1], Model_3D->Materials[i].Kd[2])));
                        glUniform3fv(default_3D_shaders_illum2_Ks, 1, value_ptr(vec3(Model_3D->Materials[i].Ks[0], Model_3D->Materials[i].Ks[1], Model_3D->Materials[i].Ks[2])));
                        
                        glActiveTexture(GL_TEXTURE2);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+0]);
                        glUniform1i(default_3D_shaders_illum2_mapKa, 2);

                        glActiveTexture(GL_TEXTURE3);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+1]);
                        glUniform1i(default_3D_shaders_illum2_mapKd, 3);

                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_2D, Model_3D->Textures[i*3+2]);
                        glUniform1i(default_3D_shaders_illum2_mapKs, 4);
                        break;
                }

                glBindVertexArray(Model_3D->VAOs[i]);
                glDrawArrays(GL_TRIANGLES, 0, Model_3D->VBOs_Size[i] / 9);
            }
        }
    }

    // --------------------- Render 2D scene
    if (current_Scene2D) {
        
    }

    // --------------------- Render GUI scene
    if (current_SceneGUI) {
        
    }

    // --------------------- Final

    int Screen_Width, Screen_Height;
    glfwGetWindowSize(EAPI_MainWindow, &Screen_Width, &Screen_Height);
    if (!Screen_Width || !Screen_Height) {Screen_Width = 1; Screen_Height = 1;}
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Screen_Width, Screen_Height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(PostEffect_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, PostEffect_Texture);

    glUseProgram(postEffect_shaders);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(EAPI_MainWindow);
}