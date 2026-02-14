#version 330 core

out uint finalColor;

uniform uint ObjectIndex;

void main() {
    finalColor = ObjectIndex;
}