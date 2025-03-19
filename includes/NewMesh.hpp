#pragma once
#include <vector>
#include "./Vector3.hpp"

class NewMesh
{
private:
    std::vector<float> vertexArray;
    std::vector<unsigned int>   indicesArray;
public:
    NewMesh(/* args */);
    ~NewMesh();

    std::vector<float> GetVertexArray();
    std::vector<unsigned int> GetIndicesArray();

    void AddVertex(Vector3 vec);
    void AddIndices(unsigned int v1, unsigned int v2, unsigned int v3);
};
