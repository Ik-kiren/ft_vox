#version 420 core

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
    mat4 instMatrix;
    BSphere bbox;
} Instance_t;


// Input vertex data, different for all executions of this shader.
layout(location = 0) in int vertexData;
layout(location = 1) in int face;
layout(location = 2) in float textureLocation;
layout(location = 3) in vec3 colorVertex;
layout(location = 4) in vec3 translation;

out vec3 vertexPos;
out vec3 normal;
out vec3 fragpos;
out vec2 textureCoords;
out vec3 color;
out flat int textureIndice;

uniform vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 facesNormal[6] = vec3[6](vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1), vec3(-1, 0, 0), vec3(0, -1, 0), vec3(0, 0, -1));

void main() {
    int posX = ((vertexData >> 24) & 31);
    int posY = ((vertexData >> 19) & 31);
    int posZ = ((vertexData >> 14) & 31);
    int TextX = ((vertexData >> 9) & 31);
    int TextY = ((vertexData >> 4) & 31);
    int type = (vertexData & 15);
    textureIndice = type;
    vec3 newVertex = vec3(posX, posY, posZ);
    gl_Position = projection * view * model * vec4((newVertex + offset), 1.0);
    vertexPos = newVertex.xyz;
    fragpos = vec3(model * vec4(newVertex + offset, 1.0));
    normal = mat3(transpose(inverse(model))) * facesNormal[face];
    textureCoords = vec2(TextX, TextY);
    color = colorVertex;
}
