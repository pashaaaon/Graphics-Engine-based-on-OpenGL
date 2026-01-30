// This is an API for a Engine_LIB.a
// https://github.com/pasha-oni/Graphics-Engine-based-on-OpenGL

const char EAPI_version[] = "Engine v1.0";

// ---------- System Functions

bool EAPI_Init();

void EAPI_UpdateWindow();

void EAPI_DestroyWindow();

int EAPI_WindowIsClosed();

void EAPI_GetWindowSize(int *width, int *height);
void EAPI_SetWindowSize(int width, int height);

void EAPI_SetWindowName(const char *title);

// ---------- Engine Functions

void EAPI_GetCameraPosition(float *x, float *y, float *z);
void EAPI_SetCameraPosition(float x, float y, float z);

void EAPI_GetCameraDirection(float *x, float *y, float *z);
void EAPI_SetCameraDirection(float x, float y, float z);

void EAPI_GetCameraAngle(float *yaw, float *pitch);
void EAPI_SetCameraAngle(float yaw, float pitch);

void EAPI_CameraMoveToDirection(float x, float y);

bool EAPI_KeyCheck(int ASCII_Key_Number);
