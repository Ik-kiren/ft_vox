#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D depthMap;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * 0.1 * 600.0) / (600.0 + 0.1 - z * (600 - 0.1));
}

void main()
{ 
    float depthValue = texture(depthMap, TexCoords).r;
    //FragColor = vec4(vec3(LinearizeDepth(depthValue) / 600), 1.0);
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
