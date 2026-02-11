class EAPI_Object_3D;
class EAPI_Model_3D {
    public:
        vector<SYSTEM_MATERIAL> SYSTEM_Materials;
        vector<GLuint> SYSTEM_Textures;
        vector<GLuint> SYSTEM_VAOs;
        vector<GLuint> SYSTEM_VBOs;
        vector<unsigned int> SYSTEM_VBOs_Size;
        bool SYSTEM_TextureBool = false;
        bool SYSTEM_model_available = false;
        vector<EAPI_Object_3D*> SYSTEM_Objects_Used = {nullptr};

        bool SYSTEM_unload() {
            SYSTEM_Materials = {};
            SYSTEM_VBOs_Size = {};
            SYSTEM_TextureBool = false;

            glDeleteVertexArrays(SYSTEM_VAOs.size(), SYSTEM_VAOs.data());
            glDeleteBuffers(SYSTEM_VBOs.size(), SYSTEM_VBOs.data());
            glDeleteTextures(SYSTEM_Textures.size(), SYSTEM_Textures.data());

            SYSTEM_model_available = false;
            return true;
        }

        bool load(const char *file_dir) {
            bool check;
            glfwPollEvents();
            SYSTEM_OBJ_MODEL model = SYSTEM_OBJ_LOAD(SYSTEM_current_path + '/' + file_dir, &check);
            glfwPollEvents();
            if (!check) {return false;}
            else {SYSTEM_unload();}

            // Texture available
            SYSTEM_TextureBool = model.TEXTURES;
            
            // Materials load
            for (SYSTEM_MATERIAL &material : model.MATERIALS) {SYSTEM_Materials.push_back(material);}
            glfwPollEvents();

            // VBOs load
            for (vector<float> &polygon_group : model.POLYGON_GROUPS) {
                glfwPollEvents();

                GLuint VAO, VBO;
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                
                glBufferData(GL_ARRAY_BUFFER, polygon_group.size() * sizeof(float), polygon_group.data(), GL_STATIC_DRAW);

                SYSTEM_VAOs.push_back(VAO);
                SYSTEM_VBOs.push_back(VBO);
                SYSTEM_VBOs_Size.push_back(polygon_group.size());
            }
            glfwPollEvents();
            
            // textures load
            for (SYSTEM_MATERIAL &material : model.MATERIALS) {
                if (material.map_Ka != "") {
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    
                    int tex_width, tex_height, rnChannels;
                    unsigned char *data = stbi_load(material.map_Ka.c_str(), &tex_width, &tex_height, &rnChannels, 0);
                    if (!data) {glDeleteTextures(1, &texture); SYSTEM_unload(); return false;}
                    
                    switch (rnChannels) {
                        case 1:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                            break;
                        case 2:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                            break;
                        case 3:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                            break;
                        case 4:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                            break;
                    }
                    
                    glGenerateMipmap(GL_TEXTURE_2D);
                    stbi_image_free(data);
                    
                    SYSTEM_Textures.push_back(texture);
                }
                else {
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                    unsigned char default_texture[4] = {static_cast<unsigned char>(material.Ka[0] * 255.0f), static_cast<unsigned char>(material.Ka[1] * 255.0f), static_cast<unsigned char>(material.Ka[2] * 255.0f), 255};
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, default_texture);
                    
                    SYSTEM_Textures.push_back(texture);
                }
                glfwPollEvents();

                if (material.map_Kd != "") {
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    
                    int tex_width, tex_height, rnChannels;
                    unsigned char *data = stbi_load(material.map_Kd.c_str(), &tex_width, &tex_height, &rnChannels, 0);
                    if (!data) {glDeleteTextures(1, &texture); SYSTEM_unload(); return false;}
                    
                    switch (rnChannels) {
                        case 1:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                            break;
                        case 2:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                            break;
                        case 3:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                            break;
                        case 4:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                            break;
                    }

                    glGenerateMipmap(GL_TEXTURE_2D);
                    stbi_image_free(data);
                    
                    SYSTEM_Textures.push_back(texture);
                }
                else {
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                    unsigned char default_texture[4] = {static_cast<unsigned char>(material.Kd[0] * 255.0f), static_cast<unsigned char>(material.Kd[1] * 255.0f), static_cast<unsigned char>(material.Kd[2] * 255.0f), 255};
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, default_texture);
                    
                    SYSTEM_Textures.push_back(texture);
                }
                glfwPollEvents();

                if (material.map_Ks != "") {
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    
                    int tex_width, tex_height, rnChannels;
                    unsigned char *data = stbi_load(material.map_Ks.c_str(), &tex_width, &tex_height, &rnChannels, 0);
                    if (!data) {glDeleteTextures(1, &texture); SYSTEM_unload(); return false;}
                    
                    switch (rnChannels) {
                        case 1:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                            break;
                        case 2:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                            break;
                        case 3:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                            break;
                        case 4:
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                            break;
                    }

                    glGenerateMipmap(GL_TEXTURE_2D);
                    stbi_image_free(data);
                    
                    SYSTEM_Textures.push_back(texture);
                }
                else {
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                    unsigned char default_texture[4] = {static_cast<unsigned char>(material.Ks[0] * 255.0f), static_cast<unsigned char>(material.Ks[1] * 255.0f), static_cast<unsigned char>(material.Ks[2] * 255.0f), 255};
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, default_texture);
                    
                    SYSTEM_Textures.push_back(texture);
                }
                glfwPollEvents();
            }
            
            SYSTEM_model_available = true;
            return true;
        }

        bool load_status() {return SYSTEM_model_available;}

        void texture_linear(bool mode = true) {
            if (!mode) {
                for (GLuint tex : SYSTEM_Textures) {
                    glBindTexture(GL_TEXTURE_2D, tex);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
            }
            else {
                for (GLuint tex : SYSTEM_Textures) {
                    glBindTexture(GL_TEXTURE_2D, tex);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
            }
        }

        EAPI_Model_3D(const char *file_dir) {
            SYSTEM_model_available = load(file_dir);
        }
        ~EAPI_Model_3D();
};

class EAPI_Scene_3D;
class EAPI_Object_3D {
    public:
        EAPI_Model_3D *SYSTEM_current_model;

        float position_x = 0.0f;
        float position_y = 0.0f;
        float position_z = 0.0f;

        float rotate_angle_x = 0.0f;
        float rotate_angle_y = 0.0f;
        float rotate_angle_z = 0.0f;

        float scale_x = 1.0f;
        float scale_y = 1.0f;
        float scale_z = 1.0f;

        EAPI_Scene_3D *SYSTEM_scene = nullptr;
        unsigned int SYSTEM_index_in_scene = 0;
        unsigned int SYSTEM_index_in_model = 0;

        EAPI_Object_3D(EAPI_Model_3D *model) {
            SYSTEM_current_model = model;
            if (model) {
                SYSTEM_index_in_model = model->SYSTEM_Objects_Used.size();
                model->SYSTEM_Objects_Used.push_back(this);
            }
        }
        ~EAPI_Object_3D();

        void switch_model(EAPI_Model_3D *model) {
            if (SYSTEM_current_model) {SYSTEM_current_model->SYSTEM_Objects_Used[SYSTEM_index_in_model] = nullptr;}

            SYSTEM_current_model = model;
            if (model) {
                SYSTEM_index_in_model = model->SYSTEM_Objects_Used.size();
                model->SYSTEM_Objects_Used.push_back(this);
            }
        }
        
        EAPI_Model_3D *get_model() {return SYSTEM_current_model;}
};

class EAPI_Light_3D {
    public:
        float position_x = 0.0f;
        float position_y = 0.0f;
        float position_z = 0.0f;

        float direction_angle_yaw = 90.0f;
        float direction_angle_pitch = 0.0f;

        float color_Red = 255.0f;
        float color_Green = 255.0f;
        float color_Blue = 255.0f;

        float strength = 1.0f;
        float type = 0.0f;

        EAPI_Scene_3D *SYSTEM_scene = nullptr;
        unsigned int SYSTEM_index_in_scene = 0;

        ~EAPI_Light_3D();

        void SetLightType_spot() {type = 0.0f;};

        void SetLightType_point() {type = 1.0f;}

        void set_direction(float x, float y, float z) {
            using namespace glm;

            vec3 new_dir;
            new_dir.x = x;
            new_dir.y = y;
            new_dir.z = z;

            if (new_dir == vec3(0.0f, 0.0f, 0.0f) && vec3(position_x, position_y, position_z) == vec3(0.0f, 0.0f, 0.0f)) {
                new_dir = vec3(0.0f, 1.0f, 0.0f);
            }
            else if (new_dir == vec3(0.0f, 0.0f, 0.0f)) {new_dir = vec3(-position_x, -position_y, -position_z);}

            new_dir = normalize(new_dir);

            direction_angle_yaw = degrees(atan(new_dir.y, new_dir.x));
            direction_angle_pitch = degrees(asin(new_dir.z));
        }
};

class EAPI_Scene_3D {
    public:
        vector<EAPI_Object_3D*> SYSTEM_objects = {nullptr};
        vector<EAPI_Light_3D*> SYSTEM_lights = {nullptr};
        GLuint SYSTEM_light_buffer;
        GLuint SYSTEM_light_texture;
        int SYSTEM_light_buffer_size = 1;

        EAPI_Scene_3D() {
            glGenBuffers(1, &SYSTEM_light_buffer);
            glBindBuffer(GL_TEXTURE_BUFFER, SYSTEM_light_buffer);
            glBufferData(GL_TEXTURE_BUFFER, 1, nullptr, GL_DYNAMIC_DRAW);

            glGenTextures(1, &SYSTEM_light_texture);
            glBindTexture(GL_TEXTURE_BUFFER, SYSTEM_light_texture);
            glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, SYSTEM_light_buffer);
        }

        void update_light() {
            glBindBuffer(GL_TEXTURE_BUFFER, SYSTEM_light_buffer);

            vector<float> light_data;
            for (EAPI_Light_3D *light : SYSTEM_lights) {
                using namespace glm;

                if (!light) {continue;}

                float yaw = light->direction_angle_yaw;
                float pitch = light->direction_angle_pitch;
                
                vec3 new_dir;
                new_dir.x = cos(radians(yaw)) * cos(radians(pitch));
                new_dir.z = sin(radians(pitch));
                new_dir.y = sin(radians(yaw)) * cos(radians(pitch));
                new_dir = normalize(new_dir);

                light_data.push_back(light->position_x);
                light_data.push_back(light->position_y);
                light_data.push_back(light->position_z);

                light_data.push_back(new_dir.x);
                light_data.push_back(new_dir.y);
                light_data.push_back(new_dir.z);

                light_data.push_back(light->color_Red/255.0f);
                light_data.push_back(light->color_Green/255.0f);
                light_data.push_back(light->color_Blue/255.0f);

                light_data.push_back(light->strength);
                light_data.push_back(light->type);
            }
            
            int light_data_size = light_data.size();
            if (SYSTEM_light_buffer_size == light_data_size) {
                glBufferSubData(GL_TEXTURE_BUFFER, 0, light_data_size * sizeof(float), light_data.data());
            }
            else {
                SYSTEM_light_buffer_size = light_data_size;
                glBufferData(GL_TEXTURE_BUFFER, light_data_size * sizeof(float), light_data.data(), GL_DYNAMIC_DRAW);
            }
        }

// -------------------------------------------------------------

        void remove_all_objects() {
            for (EAPI_Object_3D *object : SYSTEM_objects) {
                if (object) {object->SYSTEM_scene = nullptr;}
            }
            SYSTEM_objects = {nullptr};
        }

        void remove_all_lights() {
            for (EAPI_Light_3D *light : SYSTEM_lights) {
                if (light) {light->SYSTEM_scene = nullptr;}
            }
            SYSTEM_lights = {nullptr};
        }

// -------------------------------------------------------------

        void add_object(EAPI_Object_3D *object) {
            if (object->SYSTEM_scene) {return;}

            object->SYSTEM_scene = this;
            object->SYSTEM_index_in_scene = SYSTEM_objects.size();
            SYSTEM_objects.push_back(object);
        }

        void remove_object(EAPI_Object_3D *object) {
            if (!object->SYSTEM_scene) {return;}

            object->SYSTEM_scene = nullptr;
            SYSTEM_objects[object->SYSTEM_index_in_scene] = nullptr;
        }

// -------------------------------------------------------------

        void add_light(EAPI_Light_3D *light) {
            if (light->SYSTEM_scene) {return;}

            light->SYSTEM_scene = this;
            light->SYSTEM_index_in_scene = SYSTEM_lights.size();
            SYSTEM_lights.push_back(light);
        }

        void remove_light(EAPI_Light_3D *light) {
            if (!light->SYSTEM_scene) {return;}

            light->SYSTEM_scene = nullptr;
            SYSTEM_lights[light->SYSTEM_index_in_scene] = nullptr;
        }

// -------------------------------------------------------------

        ~EAPI_Scene_3D() {
            SYSTEM_current_Scene3D = nullptr;

            remove_all_objects();
            remove_all_lights();

            glDeleteBuffers(1, &SYSTEM_light_buffer);
            glDeleteTextures(1, &SYSTEM_light_texture);
        }
};

EAPI_Object_3D::~EAPI_Object_3D() {
    if (SYSTEM_scene) {SYSTEM_scene->remove_object(this);}
    if (SYSTEM_current_model) {SYSTEM_current_model->SYSTEM_Objects_Used[SYSTEM_index_in_model] = nullptr;}
}

EAPI_Light_3D::~EAPI_Light_3D() {
    if (SYSTEM_scene) {SYSTEM_scene->remove_light(this);}
}

EAPI_Model_3D::~EAPI_Model_3D() {
    for (EAPI_Object_3D *object : SYSTEM_Objects_Used) {
        if (object) {object->SYSTEM_current_model = nullptr;}
    }
    SYSTEM_unload();
}

void EAPI_SelectScene3D(EAPI_Scene_3D *Scene) {SYSTEM_current_Scene3D = Scene;}