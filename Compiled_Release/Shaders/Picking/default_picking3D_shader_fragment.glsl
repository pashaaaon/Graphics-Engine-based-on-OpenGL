#version 330 core

out uvec3 finalColor;

uniform uint object_index;
uniform int type; 

void main() {
    switch (type) {
        case 0:
            finaColor = uvec3(object_index, 0, 0);
            break;
        case 1:
            finaColor = uvec3(0, object_index, 0);
            break;
        case 2:
            finaColor = uvec3(0, 0, object_index);
            break;
        default:
            finalColor = uvec3(0, 0, 0);
            break;
    }
}