#pragma once
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include "./Chunk.hpp"
#include <unordered_map>

class ChunkManager
{
private:
    std::vector<Chunk *> chunkList;
    std::vector<Chunk *> loadList;
    std::vector<Chunk *> threadList;
    std::vector<Chunk *> setupList;
    std::vector<Chunk *> visibilityList;
    std::vector<Chunk *> renderList;
    Renderer *renderer;
    Camera *camera;
    
    Vector3 lastCamPos;
    std::vector<std::future<Chunk *>> futureList;

    std::mutex listLock;

    bool threading;

public:
    std::unordered_map<Vector3, Chunk *> chunkMap;
    Vector3 maxPos;
    Vector3 minPos;
    ChunkManager(Renderer *renderer);
    ~ChunkManager();

    void Init();
    void LoadChunk();
    Chunk *LoadThread(Chunk *chunk);
    void ChunkSetup();
    void ChunkVisibility(Camera *camera);
};
