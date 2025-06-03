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
    const Vector3 directionVecs[6] = {Vector3(1, 0, 0), Vector3(-1, 0, 0), Vector3(0, 1, 0), Vector3(0, -1, 0), Vector3(0, 0, 1), Vector3(0, 0, -1)};
public:
    bool activated;
    bool loaded;
    bool meshed;
    bool update;
    Renderer *renderer;
    unsigned int meshID;
    const unsigned int UINTMAX = 4294967295;
    static const int CHUNK_SIZE_X = 16;
    static const int CHUNK_SIZE_Y = 16;
    static const int CHUNK_SIZE_Z = 16;

    Chunk();
    Chunk(Renderer *renderer, ChunkManager *chunkManager, unsigned char ***test);
    ~Chunk();

    void NewChunk(unsigned char ***test);
    void CleanChunk(bool withPos);
    Block ***GetBlocksArray();
    void CreateCube(Vector3 &normalizedPos, int &x, int &y, int &z, bool &xPositif, bool &xNegatif, bool &yPositif, bool &yNegatif, bool &zPositif, bool &zNegatif, int type);
    void CreateMesh();
    void UpdateMesh();
    void Translation(Vector3 vec);
    Vector3 GetNormalizedPos();
    Vector3 GetPosition();
    bool CheckIce(Block &nextBlock, Block *block);
    bool CubeRayCast(Camera *camera);
    bool CubeRayCast(Camera *camera, Vector3i &cubePos);

    void PrintBlocksArray();
};

