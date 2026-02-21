#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 fragPos;
out vec3 fragTexCoord;
out vec3 fragNormal;

uniform mat4 ProjView;
uniform mat4 model;

void main() {
    gl_Position = ProjView * model * vec4(aPos, 1.0f);

    fragPos = vec3(model * vec4(aPos, 1.0f));
    fragTexCoord = aTexCoord;
    fragNormal = normalize(transpose(inverse(mat3(model))) * aNormal);
}