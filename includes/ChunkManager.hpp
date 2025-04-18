#pragma once
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include "./Chunk.hpp"
#include "./mapGP.hpp"
#include "./utils.hpp"
#include "../includes/Vector3i.hpp"
#include <unordered_map>

class ChunkManager
{
private:
    std::vector<Chunk *> chunkList;
    std::vector<Chunk *> loadList;
    std::vector<Chunk *> setupList;
    std::vector<Chunk *> visibilityList;
    std::vector<Chunk *> renderList;
    Renderer *renderer;
    Camera *camera;
    
    Vector3 lastCamPos;
    Vector3 lastCamDirection;

	mapGP	tab;

public:
    std::unordered_map<Vector3, Chunk *> chunkMap;
    Vector3 maxPos;
    Vector3 minPos;
    ChunkManager(Renderer *renderer, mapGP &tab);
    ~ChunkManager();

    void Init();
    void LoadChunk();
    Chunk *LoadThread(Chunk *chunk);
    void ChunkSetup();
    void ChunkVisibility(Camera *camera);
    void UnloadChunkX(int x);
    void UnloadChunkZ(int z);
	void loadNewChunk(chunk ***toLoad, int xdiff, int zdiff);

    Vector3 GetMaxChunkPos();
    Vector3 GetMinChunkPos();
	void loadNewChunk(chunk *toLoad, int xdiff, int zdiff);
	void loadNewLine(int oldx, int newx, int z);
	void loadNewColumn(int oldz, int newz, int x);
};
