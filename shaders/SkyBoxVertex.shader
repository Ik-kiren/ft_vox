#version 420 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexData;

out vec3 texPos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    texPos = vertexData;
    mat4 viewLessTranslate = mat4(mat3(view));
    gl_Position = projection * viewLessTranslate * vec4(vertexData, 1.0);
}
