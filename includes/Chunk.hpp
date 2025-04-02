#pragma once
#include "./Block.hpp"
#include "./Vector3.hpp"
#include "./Camera.hpp"

class Renderer;

class Chunk
{
private:
    Block ***blocksArray;
    Vector3 position;
public:
    bool loaded;
    Renderer *renderer;
    unsigned int meshID;
    static const int CHUNK_SIZE = 16;

    Chunk();
    Chunk(Renderer *renderer);
    ~Chunk();

    Block ***GetBlocksArray();
    void CreateCube(int x, int y, int z, bool xPositif, bool xNegatif, bool yPositif, bool yNegatif, bool zPositif, bool zNegatif, float type);
    void CreateMesh();
    void Update();
    void Render();
    void Translation(Vector3 vec);
};

