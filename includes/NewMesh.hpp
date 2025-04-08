#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./Vector2.hpp"

class NewMesh
{
private:
    std::vector<float> vertexArray;
    std::vector<unsigned int>   indicesArray;
    Vector3 position;
    
public:
    Vector2 textureVertices[4] = {Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(0, 1)};
    unsigned int    textureIndex;
    unsigned int    VAO;
    unsigned int    VBO;
    unsigned int    EBO;

    NewMesh();
    ~NewMesh();

    std::vector<float> GetVertexArray();
    std::vector<unsigned int> GetIndicesArray();

    void AddVertex(Vector3 &vec);
    void AddVertex(Vector2 vec);
    void AddVertex(float f1, float f2);
    void AddVertex(float x, float y, float z);
    void AddFloat(float nbr);
    void AddIndices(unsigned int v1, unsigned int v2, unsigned int v3);

    void    SetPosition(Vector3 vec);
    Vector3 GetPosition();
};
