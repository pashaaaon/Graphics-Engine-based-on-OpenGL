#version 330 core

in vec3 fragPos;
in vec3 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;

uniform float d;
uniform vec3 Kd;
uniform sampler2D mapKd;
uniform bool TextureBool;

void main() {
    vec4 current_Kd;

    if (!TextureBool) {current_Kd = vec4(Kd, 1.0f);}
    else {current_Kd = texture(mapKd, vec2(fragTexCoord));}

    finalColor = vec4(current_Kd.rgb, current_Kd.a * d);
}