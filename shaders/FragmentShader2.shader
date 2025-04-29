#version 420 core

out vec4 fragText;
in vec3 vertexPos;
in vec3 normal;
in vec3 fragpos;
in vec2 textureCoords;
in vec3 color;
in float textureIndice;

uniform vec3 cameraPos;

uniform float timeValue;
uniform sampler2D dirtTexture;
uniform sampler2D stoneTexture;
uniform sampler2D sandTexture;
uniform sampler2D redSandTexture;
uniform sampler2D snowTexture;
uniform sampler2D iceTexture;
uniform sampler2D gravelTexture;
uniform sampler2D waterTexture;
uniform sampler2D oldGrassTexture;
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

	vec3 result = ambient + diffuse + specular;
	vec4 tmpTexture = texture(dirtTexture, textureCoords);
	if (textureIndice == 2) {
		tmpTexture = texture(stoneTexture, textureCoords);
	} else if (textureIndice == 3) {
		tmpTexture = texture(sandTexture, textureCoords);
	} else if (textureIndice == 4) {
		tmpTexture = texture(redSandTexture, textureCoords);
	} else if (textureIndice == 5) {
		tmpTexture = texture(snowTexture, textureCoords);
	} else if (textureIndice == 6) {
		tmpTexture = texture(iceTexture, textureCoords);
	} else if (textureIndice == 7) {
		tmpTexture = texture(gravelTexture, textureCoords);
	} else if (textureIndice == 8) {
		tmpTexture = texture(oldGrassTexture, textureCoords);
	} else if (textureIndice == 9) {
		tmpTexture = texture(waterTexture, textureCoords);
	}
	fragText = tmpTexture * vec4(result, 1);
}