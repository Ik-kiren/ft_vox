#version 420 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec3 vertexPos;

uniform vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec3 newVertex = vertexPosition_modelspace;
    newVertex = newVertex;
    gl_Position = projection * view * model * vec4(newVertex, 1.0);
    vertexPos = vertexPosition_modelspace.xyz;
}
