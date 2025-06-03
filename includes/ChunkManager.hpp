#pragma once
#include <vector>
#include "./Chunk.hpp"
#include "./mapGP.hpp"
#include "./utils.hpp"
#include "./Player.hpp"
#include "./Vector3i.hpp"
#include <unordered_map>
#include <stack>

class ChunkManager
{
private:
    std::vector<Chunk *> loadList;
    std::unordered_map<Vector3, Chunk *> setupList;
    std::unordered_map<Vector3, Chunk *> visibilityList;
    std::vector<Chunk *> renderList;

    std::stack<Chunk *> cleanChunkList;
    Renderer *renderer;
    Camera *camera;
    Player *player;
    
    Vector3 lastCamPos;
    Vector3 lastCamDirection;

	mapGP			*tab;
	unsigned char	****_chunk;
    bool newChunksAdded;
    double keyCooldown;

    void UpdateChunk(Vector3 chunkPos);
public:
    const int RENDERSIZE = 12;
    std::unordered_map<Vector3, Chunk *> chunkMap;
    Vector3 maxPos;
    Vector3 minPos;
    ChunkManager(Renderer *renderer, mapGP *tab, Player *player);
    ~ChunkManager();

    void Init();
    void LoadChunk();
    void ChunkSetup();
    void ChunkVisibility(GLFWwindow *window);
    void UnloadChunkX(int x);
    void UnloadChunkZ(int z);
    void ChunkManagerLoop(GLFWwindow *window);
    void deactivateChunkX(int xdiff);
    void deactivateChunkZ(int zdiff);
    void AddTrailChunk(int xdiff, int zdiff);
    void UpdateChunk(int xdiff, int zdiff);
    void GetLimitChunk(int xdiff, int zdiff);
    void RecycleChunk(int xdiff, int zdiff);


    Vector3 GetMaxChunkPos();
    Vector3 GetMinChunkPos();
	void loadNewChunk(int xdiff, int zdiff);
	void loadNewLine(int oldx, int newx, int z, Player *player);
	void loadNewColumn(int oldz, int newz, int x, Player *player);

    void SetCamera(Camera *camera);
	void deleteCube(Camera &camera);
};
