// API for Engine_LIB.a (1.0v-beta)
// https://github.com/pasha-oni/Graphics-Engine-based-on-OpenGL

// ---------- System Functions

bool EAPI_Init(bool Shaders_InfoLog = false, bool ScreenBuffer_Linear = true);

void EAPI_UpdateWindow();

void EAPI_DestroyWindow();

int EAPI_WindowIsClosed();

void EAPI_GetWindowSize(int *width, int *height);
void EAPI_SetWindowSize(int width, int height);

void EAPI_SetWindowName(const char *title);

void EAPI_GetVersion(const char *version[32]);

// ---------- Engine Functions

void EAPI_GetCameraPosition(float *x, float *y, float *z);
void EAPI_SetCameraPosition(float x, float y, float z);

void EAPI_GetCameraDirection(float *x, float *y, float *z);
void EAPI_SetCameraDirection(float x, float y, float z);

void EAPI_GetCameraAngle(float *yaw, float *pitch);
void EAPI_SetCameraAngle(float yaw, float pitch);

void EAPI_CameraMoveToDirection(float x, float y);

bool EAPI_KeyCheck(int ASCII_Key_Number);

void EAPI_ClearColor(float Red, float Green, float Blue);

// ---------- Graphics 3D

class EAPI_Model_3D {
    public:
        bool unload();
        bool load(const char *file_dir);
        bool load_status();
        bool texture_linear(bool mode = true);

        EAPI_Model_3D(const char *file_dir);
};

class EAPI_Object_3D {
    public:
        EAPI_Model_3D *current_model;

        float position_x = 0.0f;
        float position_y = 0.0f;
        float position_z = 0.0f;

        float rotate_angle_x = 0.0f;
        float rotate_angle_y = 0.0f;
        float rotate_angle_z = 0.0f;

        float scale_x = 1.0f;
        float scale_y = 1.0f;
        float scale_z = 1.0f;

        EAPI_Object_3D(EAPI_Model_3D *model);

        void switch_model(EAPI_Model_3D *model);
        
        EAPI_Model_3D *get_model();
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

        void SetLightType_spot();

        void SetLightType_point();

        void set_direction(float x, float y, float z);
};

class EAPI_Scene_3D {
    public:
        EAPI_Scene_3D();
        virtual ~EAPI_Scene_3D();

        void update_light();

        void remove_all_objects();

        void remove_all_lights();
};

// ---------- Graphics 2D

// ---------- Graphics GUI

// ---------- Render

void EAPI_Render(unsigned short Custom_Buffer_Width = false, unsigned short Custom_Buffer_Height = false);