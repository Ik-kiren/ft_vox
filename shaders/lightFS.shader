#version 420 core

out vec4 vertexColor;
in vec3 vertexPos;

uniform sampler2D sun;

void main()
{
	vec2 newVertex = vec2(vertexPos.y / 5, vertexPos.z / 5);
	vertexColor = texture(sun, newVertex);
}