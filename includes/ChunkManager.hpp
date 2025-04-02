#pragma once
#include <vector>
#include "./Chunk.hpp"

class ChunkManager
{
private:
    std::vector<Chunk *> loadList;
    std::vector<Chunk *> visibilityList;
    Renderer *renderer;
    Camera *camera;
    
    Vector3 lastCamPos;
public:
    ChunkManager(Renderer *renderer, Camera *camera);
    ~ChunkManager();

    void LoadChunk();
    void ChunkVisibility();
};
