#version 330 core

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
	vec4 tmpTexture = texture(ourTexture, textpos);
	if (textureCoords != vec2(0, 0))
		tmpTexture = vec4((ambient + diffuse + specular), 1.0) * texture(ourTexture, textureCoords);
	if (activeTexture)
		fragText = vec4(mix(result.x, tmpTexture.x, timerTextureTransition), mix(result.y, tmpTexture.y, timerTextureTransition), mix(result.z, tmpTexture.z, timerTextureTransition), 1.0);
	else
		fragText = vec4(mix(tmpTexture.x, result.x, timerTextureTransition), mix(tmpTexture.y, result.y, timerTextureTransition), mix(tmpTexture.z, result.z, timerTextureTransition), 1.0);
}