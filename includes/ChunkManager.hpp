#pragma once
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include "./Chunk.hpp"

class ChunkManager
{
private:
    std::vector<Chunk *> loadList;
    std::vector<Chunk *> threadList;
    std::vector<Chunk *> setupList;
    std::vector<Chunk *> visibilityList;
    std::vector<Chunk *> renderList;
    Renderer *renderer;
    Camera *camera;
    
    Vector3 lastCamPos;
    std::vector<std::future<Chunk *>> futureList;
    std::future<void> future;


    bool threading;
public:
    ChunkManager(Renderer *renderer, Camera *camera);
    ~ChunkManager();

    void LoadChunk();
    Chunk *LoadThread(Chunk *chunk);
    void ChunkVisibility();
};
