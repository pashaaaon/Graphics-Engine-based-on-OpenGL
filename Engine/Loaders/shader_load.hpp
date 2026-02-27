#include <string>
#include <fstream>

// ------------------------------------------------

GLuint default_3D_shaders_illum0;
GLuint default_3D_shaders_illum0_ProjView;
GLuint default_3D_shaders_illum0_model;
GLuint default_3D_shaders_illum0_TextureBool;
GLuint default_3D_shaders_illum0_d;
GLuint default_3D_shaders_illum0_Kd;
GLuint default_3D_shaders_illum0_mapKd;

GLuint default_3D_shaders_illum1;
GLuint default_3D_shaders_illum1_ProjView;
GLuint default_3D_shaders_illum1_model;
GLuint default_3D_shaders_illum1_camDirection;
GLuint default_3D_shaders_illum1_camPosition;
GLuint default_3D_shaders_illum1_LightBuffer;
GLuint default_3D_shaders_illum1_LightBufferSize;
GLuint default_3D_shaders_illum1_TextureBool;
GLuint default_3D_shaders_illum1_d;
GLuint default_3D_shaders_illum1_Ka;
GLuint default_3D_shaders_illum1_Kd;
GLuint default_3D_shaders_illum1_mapKa;
GLuint default_3D_shaders_illum1_mapKd;

GLuint default_3D_shaders_illum2;
GLuint default_3D_shaders_illum2_ProjView;
GLuint default_3D_shaders_illum2_model;
GLuint default_3D_shaders_illum2_camDirection;
GLuint default_3D_shaders_illum2_camPosition;
GLuint default_3D_shaders_illum2_LightBuffer;
GLuint default_3D_shaders_illum2_LightBufferSize;
GLuint default_3D_shaders_illum2_TextureBool;
GLuint default_3D_shaders_illum2_d;
GLuint default_3D_shaders_illum2_Ns;
GLuint default_3D_shaders_illum2_Ka;
GLuint default_3D_shaders_illum2_Kd;
GLuint default_3D_shaders_illum2_Ks;
GLuint default_3D_shaders_illum2_mapKa;
GLuint default_3D_shaders_illum2_mapKd;
GLuint default_3D_shaders_illum2_mapKs;

// ------------------------------------------------

GLuint default_2D_shaders;
GLuint default_2D_shaders_ProjView;
GLuint default_2D_shaders_model;

// ------------------------------------------------

GLuint default_picking_shaders;
GLuint default_picking_shaders_ProjView;
GLuint default_picking_shaders_model;
GLuint default_picking_shaders_ObjectIndex;

// ------------------------------------------------

GLuint postEffect_shaders;
GLuint postEffect_shaders_Screen;

// ------------------------------------------------

string SYSTEM_READ_FILE(std::string file_dir) {
    ifstream file_read(SYSTEM_current_path + '/' + file_dir);
    string current_string;
    string result;

    while (getline(file_read, current_string)) {
        result += current_string + '\n';
    }

    return result;
}

void SYSTEM_SHADERS_3D_LOAD(bool InfoLog = false) {
    string default_3D_shader_vertex_src_string = SYSTEM_READ_FILE("Shaders/3D/default_3D_shader_vertex.glsl");
    string default_3D_shader_fragment_illum0_src_string = SYSTEM_READ_FILE("Shaders/3D/default_3D_shader_fragment_illum0.glsl");
    string default_3D_shader_fragment_illum1_src_string = SYSTEM_READ_FILE("Shaders/3D/default_3D_shader_fragment_illum1.glsl");
    string default_3D_shader_fragment_illum2_src_string = SYSTEM_READ_FILE("Shaders/3D/default_3D_shader_fragment_illum2.glsl");

    const char *default_3D_shader_vertex_src = default_3D_shader_vertex_src_string.c_str();
    const char *default_3D_shader_fragment_illum0_src = default_3D_shader_fragment_illum0_src_string.c_str();
    const char *default_3D_shader_fragment_illum1_src = default_3D_shader_fragment_illum1_src_string.c_str();
    const char *default_3D_shader_fragment_illum2_src = default_3D_shader_fragment_illum2_src_string.c_str();

    GLuint default_3D_shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint default_3D_shader_fragment_illum0 = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint default_3D_shader_fragment_illum1 = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint default_3D_shader_fragment_illum2 = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(default_3D_shader_vertex, 1, &default_3D_shader_vertex_src, nullptr);
    glShaderSource(default_3D_shader_fragment_illum0, 1, &default_3D_shader_fragment_illum0_src, nullptr);
    glShaderSource(default_3D_shader_fragment_illum1, 1, &default_3D_shader_fragment_illum1_src, nullptr);
    glShaderSource(default_3D_shader_fragment_illum2, 1, &default_3D_shader_fragment_illum2_src, nullptr);

    glCompileShader(default_3D_shader_vertex);
    glCompileShader(default_3D_shader_fragment_illum0);
    glCompileShader(default_3D_shader_fragment_illum1);
    glCompileShader(default_3D_shader_fragment_illum2);

    // ---------------------------------------------------------------------------------------------------------------------------

    default_3D_shaders_illum0 = glCreateProgram();
    glAttachShader(default_3D_shaders_illum0, default_3D_shader_vertex);
    glAttachShader(default_3D_shaders_illum0, default_3D_shader_fragment_illum0);
    glLinkProgram(default_3D_shaders_illum0);

    default_3D_shaders_illum1 = glCreateProgram();
    glAttachShader(default_3D_shaders_illum1, default_3D_shader_vertex);
    glAttachShader(default_3D_shaders_illum1, default_3D_shader_fragment_illum1);
    glLinkProgram(default_3D_shaders_illum1);

    default_3D_shaders_illum2 = glCreateProgram();
    glAttachShader(default_3D_shaders_illum2, default_3D_shader_vertex);
    glAttachShader(default_3D_shaders_illum2, default_3D_shader_fragment_illum2);
    glLinkProgram(default_3D_shaders_illum2);

    if (InfoLog) {
        int check_illum0, check_illum1, check_illum2;

        glGetProgramiv(default_3D_shaders_illum0, GL_LINK_STATUS, &check_illum0);
        glGetProgramiv(default_3D_shaders_illum1, GL_LINK_STATUS, &check_illum1);
        glGetProgramiv(default_3D_shaders_illum2, GL_LINK_STATUS, &check_illum2);

        if (!check_illum0) {
            char message[8192];
            glGetProgramInfoLog(default_3D_shaders_illum0, 8192, NULL, message);
            cout << "\n==============================\n" << "default_3D_shaders_illum0 : " << "\n\n" << message << "==============================\n";
        }

        if (!check_illum1) {
            char message[8192];
            glGetProgramInfoLog(default_3D_shaders_illum1, 8192, NULL, message);
            cout << "\n==============================\n" << "default_3D_shaders_illum1 : " << "\n\n" << message << "==============================\n";
        }

        if (!check_illum2) {
            char message[8192];
            glGetProgramInfoLog(default_3D_shaders_illum2, 8192, NULL, message);
            cout << "\n==============================\n" << "default_3D_shaders_illum2 : " << "\n\n" << message << "==============================\n";
        }
    }

    // ---------------------------------------------------------------------------------------------------------------------------

    default_3D_shaders_illum0_ProjView = glGetUniformLocation(default_3D_shaders_illum0, "ProjView");
    default_3D_shaders_illum0_model = glGetUniformLocation(default_3D_shaders_illum0, "model");
    default_3D_shaders_illum0_TextureBool = glGetUniformLocation(default_3D_shaders_illum0, "TextureBool");
    default_3D_shaders_illum0_d = glGetUniformLocation(default_3D_shaders_illum0, "d");
    default_3D_shaders_illum0_Kd = glGetUniformLocation(default_3D_shaders_illum0, "Kd");
    default_3D_shaders_illum0_mapKd = glGetUniformLocation(default_3D_shaders_illum0, "mapKd");

    default_3D_shaders_illum1_ProjView = glGetUniformLocation(default_3D_shaders_illum1, "ProjView");
    default_3D_shaders_illum1_model = glGetUniformLocation(default_3D_shaders_illum1, "model");
    default_3D_shaders_illum1_camDirection = glGetUniformLocation(default_3D_shaders_illum1, "camDirection");
    default_3D_shaders_illum1_camPosition = glGetUniformLocation(default_3D_shaders_illum1, "camPosition");
    default_3D_shaders_illum1_LightBuffer = glGetUniformLocation(default_3D_shaders_illum1, "LightBuffer");
    default_3D_shaders_illum1_LightBufferSize = glGetUniformLocation(default_3D_shaders_illum1, "LightBufferSize");
    default_3D_shaders_illum1_TextureBool = glGetUniformLocation(default_3D_shaders_illum1, "TextureBool");
    default_3D_shaders_illum1_d = glGetUniformLocation(default_3D_shaders_illum1, "d");
    default_3D_shaders_illum1_Ka = glGetUniformLocation(default_3D_shaders_illum1, "Ka");
    default_3D_shaders_illum1_Kd = glGetUniformLocation(default_3D_shaders_illum1, "Kd");
    default_3D_shaders_illum1_mapKa = glGetUniformLocation(default_3D_shaders_illum1, "mapKa");
    default_3D_shaders_illum1_mapKd = glGetUniformLocation(default_3D_shaders_illum1, "mapKd");

    default_3D_shaders_illum2_ProjView = glGetUniformLocation(default_3D_shaders_illum2, "ProjView");
    default_3D_shaders_illum2_model = glGetUniformLocation(default_3D_shaders_illum2, "model");
    default_3D_shaders_illum2_camDirection = glGetUniformLocation(default_3D_shaders_illum2, "camDirection");
    default_3D_shaders_illum2_camPosition = glGetUniformLocation(default_3D_shaders_illum2, "camPosition");
    default_3D_shaders_illum2_LightBuffer = glGetUniformLocation(default_3D_shaders_illum2, "LightBuffer");
    default_3D_shaders_illum2_LightBufferSize = glGetUniformLocation(default_3D_shaders_illum2, "LightBufferSize");
    default_3D_shaders_illum2_TextureBool = glGetUniformLocation(default_3D_shaders_illum2, "TextureBool");
    default_3D_shaders_illum2_d = glGetUniformLocation(default_3D_shaders_illum2, "d");
    default_3D_shaders_illum2_Ns = glGetUniformLocation(default_3D_shaders_illum2, "Ns");
    default_3D_shaders_illum2_Ka = glGetUniformLocation(default_3D_shaders_illum2, "Ka");
    default_3D_shaders_illum2_Kd = glGetUniformLocation(default_3D_shaders_illum2, "Kd");
    default_3D_shaders_illum2_Ks = glGetUniformLocation(default_3D_shaders_illum2, "Ks");
    default_3D_shaders_illum2_mapKa = glGetUniformLocation(default_3D_shaders_illum2, "mapKa");
    default_3D_shaders_illum2_mapKd = glGetUniformLocation(default_3D_shaders_illum2, "mapKd");
    default_3D_shaders_illum2_mapKs = glGetUniformLocation(default_3D_shaders_illum2, "mapKs");
}

void SYSTEM_SHADERS_2D_LOAD(bool InfoLog = false) {
    string default_2D_shader_vertex_src_string = SYSTEM_READ_FILE("Shaders/2D/default_2D_shader_vertex.glsl");
    string default_2D_shader_fragment_src_string = SYSTEM_READ_FILE("Shaders/2D/default_2D_shader_fragment.glsl");

    const char *default_2D_shader_vertex_src = default_2D_shader_vertex_src_string.c_str();
    const char *default_2D_shader_fragment_src = default_2D_shader_fragment_src_string.c_str();

    GLuint default_2D_shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint default_2D_shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(default_2D_shader_vertex, 1, &default_2D_shader_vertex_src, nullptr);
    glShaderSource(default_2D_shader_fragment, 1, &default_2D_shader_fragment_src, nullptr);

    glCompileShader(default_2D_shader_vertex);
    glCompileShader(default_2D_shader_fragment);

    // ---------------------------------------------------------------------------------------------------------------------------

    default_2D_shaders = glCreateProgram();
    glAttachShader(default_2D_shaders, default_2D_shader_vertex);
    glAttachShader(default_2D_shaders, default_2D_shader_fragment);
    glLinkProgram(default_2D_shaders);

    if (InfoLog) {
        int check_default;
        glGetProgramiv(default_2D_shaders, GL_LINK_STATUS, &check_default);

        if (!check_default) {
            char message[8192];
            glGetProgramInfoLog(default_2D_shaders, 8192, NULL, message);
            cout << "\n==============================\n" << "default_2D_shaders : " << "\n\n" << message << "==============================\n";
        }
    }

    // ---------------------------------------------------------------------------------------------------------------------------

    default_2D_shaders_ProjView = glGetUniformLocation(default_2D_shaders, "ProjView");
    default_2D_shaders_model = glGetUniformLocation(default_2D_shaders, "model");
}

void SYSTEM_SHADERS_PICKING_LOAD(bool InfoLog = false) {
    string default_picking_shader_vertex_src_string = SYSTEM_READ_FILE("Shaders/Picking/default_picking_shader_vertex.glsl");
    string default_picking_shader_fragment_src_string = SYSTEM_READ_FILE("Shaders/Picking/default_picking_shader_fragment.glsl");

    const char *default_picking_shader_vertex_src = default_picking_shader_vertex_src_string.c_str();
    const char *default_picking_shader_fragment_src = default_picking_shader_fragment_src_string.c_str();

    GLuint default_picking_shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint default_picking_shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(default_picking_shader_vertex, 1, &default_picking_shader_vertex_src, nullptr);
    glShaderSource(default_picking_shader_fragment, 1, &default_picking_shader_fragment_src, nullptr);

    glCompileShader(default_picking_shader_vertex);
    glCompileShader(default_picking_shader_fragment);

    // ---------------------------------------------------------------------------------------------------------------------------

    default_picking_shaders = glCreateProgram();
    glAttachShader(default_picking_shaders, default_picking_shader_vertex);
    glAttachShader(default_picking_shaders, default_picking_shader_fragment);
    glLinkProgram(default_picking_shaders);

    if (InfoLog) {
        int check_default;
        glGetProgramiv(default_picking_shaders, GL_LINK_STATUS, &check_default);

        if (!check_default) {
            char message[8192];
            glGetProgramInfoLog(default_picking_shaders, 8192, NULL, message);
            cout << "\n==============================\n" << "default_picking_shaders : " << "\n\n" << message << "==============================\n";
        }
    }

    // ---------------------------------------------------------------------------------------------------------------------------

    default_picking_shaders_ProjView = glGetUniformLocation(default_picking_shaders, "ProjView");
    default_picking_shaders_model = glGetUniformLocation(default_picking_shaders, "model");
    default_picking_shaders_ObjectIndex = glGetUniformLocation(default_picking_shaders, "ObjectIndex");
}

void SYSTEM_SHADERS_POSTEFFECT_LOAD(bool InfoLog = false) {
    string postEffect_shader_vertex_src_string = SYSTEM_READ_FILE("Shaders/PostEffect/postEffect_shader_vertex.glsl");
    string postEffect_shader_fragment_src_string = SYSTEM_READ_FILE("Shaders/PostEffect/postEffect_shader_fragment.glsl");

    const char *postEffect_shader_vertex_src = postEffect_shader_vertex_src_string.c_str();
    const char *postEffect_shader_fragment_src = postEffect_shader_fragment_src_string.c_str();

    GLuint postEffect_shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint postEffect_shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(postEffect_shader_vertex, 1, &postEffect_shader_vertex_src, nullptr);
    glShaderSource(postEffect_shader_fragment, 1, &postEffect_shader_fragment_src, nullptr);

    glCompileShader(postEffect_shader_vertex);
    glCompileShader(postEffect_shader_fragment);

    // ---------------------------------------------------------------------------------------------------------------------------

    postEffect_shaders = glCreateProgram();
    glAttachShader(postEffect_shaders, postEffect_shader_vertex);
    glAttachShader(postEffect_shaders, postEffect_shader_fragment);
    glLinkProgram(postEffect_shaders);

    if (InfoLog) {
        int check_default;
        glGetProgramiv(postEffect_shaders, GL_LINK_STATUS, &check_default);

        if (!check_default) {
            char message[8192];
            glGetProgramInfoLog(postEffect_shaders, 8192, NULL, message);
            cout << "\n==============================\n" << "postEffect_shaders : " << "\n\n" << message << "==============================\n";
        }
    }

    // ---------------------------------------------------------------------------------------------------------------------------

    postEffect_shaders_Screen = glGetUniformLocation(postEffect_shaders, "Screen");
}