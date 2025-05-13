#pragma once
#include <vector>
#include "./Chunk.hpp"
#include "./mapGP.hpp"
#include "./utils.hpp"
#include "./Player.hpp"
#include "./Vector3i.hpp"
#include <unordered_map>

class ChunkManager
{
private:
    std::vector<Chunk *> loadList;
    std::vector<Chunk *> setupList;
    std::unordered_map<Vector3, Chunk *> unloadMap;
    std::vector<Chunk *> visibilityList;
    std::vector<Chunk *> renderList;
    Renderer *renderer;
    Camera *camera;
    Player *player;
    
    Vector3 lastCamPos;
    Vector3 lastCamDirection;

	mapGP	*tab;

public:
    const int RENDERSIZE = 16;
    std::unordered_map<Vector3, Chunk *> chunkMap;
    Vector3 maxPos;
    Vector3 minPos;
    ChunkManager(Renderer *renderer, mapGP *tab, Player *player);
    ~ChunkManager();

    void Init();
    void LoadChunk();
    Chunk *LoadThread(Chunk *chunk);
    void ChunkSetup();
    void ChunkUnload();
    void ChunkVisibility();
    void UnloadChunkX(int x);
    void UnloadChunkZ(int z);
    void ChunkManagerLoop();
    void deactivateChunkX(int xdiff);
    void deactivateChunkZ(int zdiff);
    void AddTrailChunk(chunk *toLoad, int xdiff, int zdiff);
    void UpdateChunk(int xdiff, int zdiff);
    void GetLimitChunk(int xdiff, int zdiff);


    Vector3 GetMaxChunkPos();
    Vector3 GetMinChunkPos();
	void loadNewChunk(chunk *toLoad, int xdiff, int zdiff);
	void loadNewLine(int oldx, int newx, int z, Player *player);
	void loadNewColumn(int oldz, int newz, int x, Player *player);

    void SetCamera(Camera *camera);
	void deleteCube(Camera &camera);
};
