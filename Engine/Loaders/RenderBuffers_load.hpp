// PostEffect Buffer

GLuint SYSTEM_PostEffect_FrameBuffer;
GLuint SYSTEM_PostEffect_DepthBuffer;
GLuint SYSTEM_PostEffect_Texture;
GLuint SYSTEM_PostEffect_VAO;
GLuint SYSTEM_PostEffect_VBO;
vector<float> SYSTEM_PostEffect_Texture_Coords = {
    -1.0f, -1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f,   0.0f, 1.0f,
    1.0f, -1.0f,   1.0f, 0.0f,

    1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f, 1.0f,   0.0f, 1.0f,
    1.0f, -1.0f,   1.0f, 0.0f
};

void SYSTEM_PostEffect_INIT(bool ScreenBuffer_Linear) {
    glGenFramebuffers(1, &SYSTEM_PostEffect_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, SYSTEM_PostEffect_FrameBuffer);
    
    glGenRenderbuffers(1, &SYSTEM_PostEffect_DepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, SYSTEM_PostEffect_DepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 640, 480);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, SYSTEM_PostEffect_DepthBuffer);
    
    glGenTextures(1, &SYSTEM_PostEffect_Texture);
    glBindTexture(GL_TEXTURE_2D, SYSTEM_PostEffect_Texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    if (ScreenBuffer_Linear) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SYSTEM_PostEffect_Texture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SYSTEM_PostEffect_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glUseProgram(postEffect_shaders);
    glUniform1i(postEffect_shaders_Screen, 0);
    glGenVertexArrays(1, &SYSTEM_PostEffect_VAO);
    glGenBuffers(1, &SYSTEM_PostEffect_VBO);
    glBindVertexArray(SYSTEM_PostEffect_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, SYSTEM_PostEffect_VBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, SYSTEM_PostEffect_Texture_Coords.size() * sizeof(float), SYSTEM_PostEffect_Texture_Coords.data(), GL_STATIC_DRAW);
}

// Picking Buffer

GLuint SYSTEM_Picking_FrameBuffer;
GLuint SYSTEM_Picking_DepthBuffer;
GLuint SYSTEM_Picking_Texture3D;
GLuint SYSTEM_Picking_Texture2D;
GLuint SYSTEM_Picking_TextureGUI;

void SYSTEM_Picking_INIT() {
    glGenFramebuffers(1, &SYSTEM_Picking_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, SYSTEM_Picking_FrameBuffer);
    
    glGenRenderbuffers(1, &SYSTEM_Picking_DepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, SYSTEM_Picking_DepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 640, 480);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, SYSTEM_Picking_DepthBuffer);

    glGenTextures(1, &SYSTEM_Picking_Texture3D);
    glBindTexture(GL_TEXTURE_2D, SYSTEM_Picking_Texture3D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, 640, 480, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SYSTEM_Picking_Texture3D, 0);

    glGenTextures(1, &SYSTEM_Picking_Texture2D);
    glBindTexture(GL_TEXTURE_2D, SYSTEM_Picking_Texture2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, 640, 480, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, SYSTEM_Picking_Texture2D, 0);

    glGenTextures(1, &SYSTEM_Picking_TextureGUI);
    glBindTexture(GL_TEXTURE_2D, SYSTEM_Picking_TextureGUI);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, 640, 480, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, SYSTEM_Picking_TextureGUI, 0);
}