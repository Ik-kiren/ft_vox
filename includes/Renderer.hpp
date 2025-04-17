#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./NewMesh.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"
#include "./Chunk.hpp"
#include <mutex>

class Renderer
{
private:
    Shader *shader;
    Camera *camera;
    Matrix4 model;

    unsigned int atomicID;
    unsigned int atomicCounter;

    unsigned int textureID;
    unsigned int textureID2;
    unsigned int textureSSBO;

    std::vector<float> normalVertices;
    std::mutex renderMutex;
public:
    const int STRIDE_SIZE = 6;
    std::vector<NewMesh *> meshes;
    Renderer();
    ~Renderer();

    void InitRenderer(Shader *shader, Camera *camera);
    void CreateMesh(unsigned int &meshID);
    unsigned int AddVertex(unsigned int &meshID, Vector3 &vecs, float type);
    unsigned int AddVertex(unsigned int &meshID, float x, float y, float z, float type, Vector2 size);
    void addIndices(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3);
    void FinishMesh(unsigned int &meshID);
    void Render();
    void Render(unsigned int &meshID);
    void Render(std::vector<Chunk *> &chunks);
    void InitTexture();

    Renderer &operator=(const Renderer &rhs);
};