#pragma once
#include "./Block.hpp"
#include "./Vector3.hpp"
#include "./Camera.hpp"

class ChunkManager;
class Renderer;

class Chunk
{
private:
    Block ***blocksArray;
    Vector3 position;
    ChunkManager *chunkManager;
public:
    std::vector<Chunk *> *chunkList;
    bool activated;
    bool loaded;
    bool meshed;
    Renderer *renderer;
    unsigned int meshID;
    static const int CHUNK_SIZE_X = 16;
    static const int CHUNK_SIZE_Y = 16;
    static const int CHUNK_SIZE_Z = 16;

    Chunk();
    Chunk(Renderer *renderer, ChunkManager *chunkManager, unsigned char ***test);
    ~Chunk();

    Block ***GetBlocksArray();
    void CreateCube(int &x, int &y, int &z, bool &xPositif, bool &xNegatif, bool &yPositif, bool &yNegatif, bool &zPositif, bool &zNegatif, float type);
    void CreateMesh();
    void Update();
    void Render();
    void Translation(Vector3 vec);
    Vector3 GetNormalizedPos();

    Vector3 GetPosition();
};

