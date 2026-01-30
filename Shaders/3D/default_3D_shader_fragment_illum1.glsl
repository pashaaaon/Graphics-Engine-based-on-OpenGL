#version 330 core

in vec3 fragPos;
in vec3 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;

uniform float d;
uniform vec3 Ka;
uniform vec3 Kd;
uniform sampler2D mapKa;
uniform sampler2D mapKd;
uniform vec3 camDirection;
uniform vec3 camPosition;
uniform samplerBuffer LightBuffer;
uniform int LightBufferSize;
uniform bool TextureBool;

void main() {
    vec4 current_Ka;
    vec4 current_Kd;

    if (!TextureBool) {current_Ka = vec4(Ka * 0.1f, 1.0f);   current_Kd = vec4(Kd, 1.0f);}
    else {current_Ka = texture(mapKa, vec2(fragTexCoord)) * 0.1f;   current_Kd = texture(mapKd, vec2(fragTexCoord));}

    if (LightBufferSize >= 11) {
        vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
        float light_distance_strength = 0.0f;

        for (int i = 0; i<LightBufferSize; i+=11) {
            float pos_x = texelFetch(LightBuffer, i).r;
            float pos_y = texelFetch(LightBuffer, i+1).r;
            float pos_z = texelFetch(LightBuffer, i+2).r;

            float dir_x = texelFetch(LightBuffer, i+3).r;
            float dir_y = texelFetch(LightBuffer, i+4).r;
            float dir_z = texelFetch(LightBuffer, i+5).r;

            float color_r = texelFetch(LightBuffer, i+6).r;
            float color_g = texelFetch(LightBuffer, i+7).r;
            float color_b = texelFetch(LightBuffer, i+8).r;

            float strength = texelFetch(LightBuffer, i+9).r;
            float type = texelFetch(LightBuffer, i+10).r;

            vec3 lightPos = vec3(pos_x, pos_y, pos_z);
            vec3 lightDir = vec3(0.0f, 0.0f, 0.0f);
            vec3 lightColor = vec3(color_r, color_g, color_b);

            vec3 normal = fragNormal;
            if (!gl_FrontFacing) {normal = -normal;}

            if (type == 0.0f) {lightDir = vec3(dir_x, dir_y, dir_z);}
            else if (type == 1.0f) {lightDir = normalize(lightPos - fragPos);}

            diffuse += max(dot(normal, lightDir), 0.0f) * lightColor * strength;
            light_distance_strength += 1.0f / (1.0f + 0.005 * length(fragPos - lightPos));
        }

        vec3 resultColor = (current_Ka.rgb + diffuse) * current_Kd.rgb * light_distance_strength;
        finalColor = vec4(resultColor, current_Kd.a * d);
    }
    else {finalColor = vec4(current_Ka.rgb, current_Kd.a * d);}
}