#version 330 core

in vec2 fragTexCoords;
out vec4 finalColor;

uniform sampler2D Screen;

void main() {
    finalColor = texture(Screen, fragTexCoords);
}