#include "./Shader.hpp"
#include "./Chunk.hpp"
#include <unordered_map>

class Renderer;

class ShadowMap
{
private:
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int quadVAO;
    unsigned int quadVBO;
    const float quadVertices[20] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    unsigned int depthMapFBO;

    Shader debugDepthQuad;
    Shader simpleDepthShader;

public:
    unsigned int depthMap;
    Matrix4 lightSpaceMatrix;
    ShadowMap(/* args */);
    ~ShadowMap();

    void Render(Renderer *renderer, std::unordered_map<Vector3, Chunk *> &visibility);
};