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
    bool activated;
    bool loaded;
    bool meshed;
    bool unload;
    bool update;
    Renderer *renderer;
    unsigned int meshID;
    static const int CHUNK_SIZE_X = 16;
    static const int CHUNK_SIZE_Y = 16;
    static const int CHUNK_SIZE_Z = 16;

    Chunk();
    Chunk(Renderer *renderer, ChunkManager *chunkManager, unsigned char ***test);
    ~Chunk();

    Block ***GetBlocksArray();
    void CreateCube(int &x, int &y, int &z, bool &xPositif, bool &xNegatif, bool &yPositif, bool &yNegatif, bool &zPositif, bool &zNegatif, int type);
    void CreateMesh();
    void UpdateMesh();
    void Update();
    void Render();
    void Translation(Vector3 vec);
    Vector3 GetNormalizedPos();
    bool CheckXPositifIsVisible(int x, int y, int z);
    Vector3 GetPosition();
    void MeshCulling(Vector3i pos, Vector3i direction, Vector3i upVec, FacesType face);
};

