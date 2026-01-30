#version 330 core

out vec4 finalColor;

uniform int object_index;

void main() {
    finaColor = vec4(float(object_index), 0.0f, 0.0f, 1.0f);
}