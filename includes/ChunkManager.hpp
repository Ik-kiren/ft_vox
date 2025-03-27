#pragma once
#include <vector>
#include "./Chunk.hpp"

class ChunkManager
{
private:
    std::vector<Chunk *> loadList;
    Renderer *renderer;
public:
    ChunkManager(Renderer *renderer);
    ~ChunkManager();

    void LoadChunk();
};
