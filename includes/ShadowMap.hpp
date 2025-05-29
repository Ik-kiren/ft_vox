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

    unsigned int planeVAO;
    unsigned int planeVBO;
    const float PlaneVertices[18] = {
        0.0f, -160.0f, -1000.0f,
        0.0f, -160.0f, 1000.0f,
        0.0f, 160.0f, -1000.0f,

        0.0f, 160.0f, 1000.0f,
        0.0f, -160.0f, 1000.0f,
        0.0f, 160.0f, -1000.0f
    };

    unsigned int depthMapFBO;

    Shader debugDepthQuad;
    Shader simpleDepthShader;
    Shader planeShadowMapShader;

    Vector3 planDirection[4] = {
        Vector3(1.0, 0.0, 0.0), Vector3(-1.0, 0.0, 0.0)
    };

    double debugCooldown;

    void Init();
public:
    bool debug = false;
    unsigned int depthMap;
    Matrix4 lightSpaceMatrix;
    ShadowMap();
    ShadowMap(bool debug);
    ~ShadowMap();

    void SetDebug(bool debug);
    void Render(Renderer *renderer, std::unordered_map<Vector3, Chunk *> &visibility);
};