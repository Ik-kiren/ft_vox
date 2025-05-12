#version 420 core

out vec4 texColor;

in vec3 texPos;

uniform samplerCube skybox;

void main()
{
	texColor = texture(skybox, texPos);
}