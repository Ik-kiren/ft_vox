#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./NewMesh.hpp"

class Renderer
{
private:
public:
    std::vector<NewMesh> meshes;
    Renderer(/* args */);
    ~Renderer();

    void CreateMesh(unsigned int &meshID);
    unsigned int AddVertex(unsigned int meshID, Vector3 vec);
    void addIndices(unsigned int meshID, unsigned int v1, unsigned int v2, unsigned int v3);
    void FinishMesh(unsigned int meshID);
};
