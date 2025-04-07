#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./NewMesh.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"
#include "./Chunk.hpp"

class Renderer
{
private:
    Shader &shader;
    Camera &camera;
    Matrix4 model;

    unsigned int atomicID;
    unsigned int atomicCounter;

    unsigned int textureID;
    unsigned int textureID2;
    unsigned int textureSSBO;

    std::vector<float> normalVertices;
    Vector2 textureVertices[4] = {Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(0, 1)};
    int textureIndex;
    std::mutex renderMutex;
public:
    const int STRIDE_SIZE = 6;
    std::vector<NewMesh *> meshes;
    Renderer(Shader &shader, Camera &camera);
    ~Renderer();

    void InitRenderer();
    void CreateMesh(unsigned int &meshID);
    unsigned int AddVertex(unsigned int &meshID, Vector3 &vecs, float type);
    unsigned int AddVertex(unsigned int &meshID, float x, float y, float z, float type);
    void addIndices(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3);
    void FinishMesh(unsigned int &meshID);
    void Render();
    void Render(unsigned int &meshID);
    void Render(std::vector<Chunk *> &chunks);
    void InitTexture();

    Renderer &operator=(const Renderer &rhs);
};
