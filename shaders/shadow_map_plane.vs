#version 420 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexData;

uniform vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 newVertex = vertexData;
    gl_Position = projection * view * model * vec4((newVertex + offset), 1.0);
}