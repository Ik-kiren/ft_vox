#version 420 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in int vertexData;
layout(location = 1) in int face;
layout(location = 2) in float textureLocation;

out vec3 vertexPos;
out vec3 normal;
out vec3 fragpos;
out vec2 textureCoords;
out vec3 color;
out float textureIndice;
out vec4 lightSpace;

uniform vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;

vec3 facesNormal[6] = vec3[6](vec3(0, 1, 0), vec3(0, -1, 0), vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 0, 1), vec3(0, 0, -1));

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
    lightSpace = lightProjection * lightView * vec4(fragpos, 1.0);
}
