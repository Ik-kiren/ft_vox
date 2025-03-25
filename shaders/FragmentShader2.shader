#version 460 core

out vec4 fragText;
in vec3 vertexPos;
in vec3 normal;
in vec3 fragpos;
in vec2 textureCoords;
in vec3 color;

uniform vec3 cameraPos;

uniform float timeValue;
uniform sampler2D ourTexture;
uniform bool activeTexture;
uniform float timerTextureTransition;
uniform vec3 lightPos;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightDir = normalize(lightPos - fragpos);

	float ambientStrength = 0.6;
	vec3 ambient = ambientStrength * lightColor;
	vec2 textpos = vertexPos.xy;
	float diff = max(dot(normalize(normal), lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(cameraPos - fragpos);
    vec3 reflectDir = reflect(-lightDir, normalize(normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * vec3(0.8, 0.8, 0.1);
	vec4 tmpTexture = texture(ourTexture, textureCoords);
	fragText = tmpTexture;
}