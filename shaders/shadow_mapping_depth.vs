#version 420 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in int vertexData;

uniform vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    int posX = ((vertexData >> 24) & 31);
    int posY = ((vertexData >> 19) & 31);
    int posZ = ((vertexData >> 14) & 31);
    vec3 newVertex = vec3(posX, posY, posZ);
    gl_Position = projection * view * model * vec4((newVertex + offset), 1.0);
}