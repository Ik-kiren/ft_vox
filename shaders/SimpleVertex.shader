#version 460 core


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

out vec3 vertexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec3 newVertex = vertexPosition_modelspace;
    gl_Position = projection * view * model * vec4(newVertex, 1.0);
    vertexPos = newVertex.xyz;
}
