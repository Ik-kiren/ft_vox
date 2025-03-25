#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./NewMesh.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"

class Renderer
{
private:
    Shader &shader;
    Camera &camera;
    Matrix4 model;

    unsigned int atomicID;
    unsigned int atomicCounter;

    unsigned int textureID;
    unsigned int textureSSBO;

    std::vector<float> normalVertices;
    Vector2 textureVertices[4] = {Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(0, 1)};
    int textureIndex;
public:
    std::vector<NewMesh> meshes;
    Renderer(Shader &shader, Camera &camera);
    ~Renderer();

    void InitRenderer();
    void CreateMesh(unsigned int &meshID);
    unsigned int AddVertex(unsigned int meshID, Vector3 vecs);
    void addIndices(unsigned int meshID, unsigned int v1, unsigned int v2, unsigned int v3);
    void FinishMesh(unsigned int &meshID);
    void Render();
    void Render(unsigned int meshID);
    void InitTexture();

    Renderer &operator=(const Renderer &rhs);
};
