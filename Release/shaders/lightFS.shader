#version 330 core

out vec3 vertexColor;
in vec3 vertexPos;

uniform vec3 cameraPos;

uniform float timeValue;
uniform sampler2D ourTexture;
uniform bool activeTexture;
uniform float timerTextureTransition;

void main()
{
	vertexColor = vec3(1, 1, 1);
}