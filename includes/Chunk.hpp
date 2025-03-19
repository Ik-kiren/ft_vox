#pragma once
#include "./Block.hpp"
#include "./Renderer.hpp"

class Chunk
{
private:
    Block ***blocksArray;
public:
    Renderer renderer;
    unsigned int meshID;
    static const int CHUNK_SIZE = 16;

    Chunk();
    ~Chunk();

    void CreateCube(int x, int y, int z);
    void CreateMesh();
    void Update();
    void Render();
};

