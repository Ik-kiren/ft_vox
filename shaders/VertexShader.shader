#version 460 core

struct Plane {
    vec3 normal; 
    float distance;
} Plane_t;

struct Frustum {
    Plane topFace;
    Plane bottomFace;
    Plane rightFace;
    Plane leftFace;
    Plane nearFace;
    Plane farFace;
} Frustum_t;

struct BSphere {
    vec3 center;
    float radius;
} BSphere_t;

struct Instance {
    mat4 matrix;
    BSphere bbox;
} Instance_t;


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normalVertex;
layout(location = 2) in vec2 textureVertex;
layout(location = 3) in vec3 colorVertex;
layout(location = 4) in vec3 translation;

out vec3 vertexPos;
out vec3 normal;
out vec3 fragpos;
out vec2 textureCoords;
out vec3 color;

uniform vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (std430, binding = 5) buffer bufferData {
    Frustum frustum;
    uint atomicData[8];
    Instance instance[8];
};

void main(){
    vec3 newVertex = vertexPosition_modelspace;
    newVertex = newVertex;
    gl_Position = projection * view * model * vec4(newVertex, 1.0);
    vertexPos = newVertex.xyz;
    fragpos = vec3(model * vec4(newVertex, 1.0));
    normal = mat3(transpose(inverse(model))) * normalVertex;
    textureCoords = textureVertex;
    color = colorVertex;
}
