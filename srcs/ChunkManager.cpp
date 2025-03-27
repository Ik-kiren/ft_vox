#include "../includes/ChunkManager.hpp"

ChunkManager::ChunkManager(Renderer *renderer): renderer(renderer) {
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 6; k++) {
                Chunk *newChunk = new Chunk(renderer);
                newChunk->Translation(Vector3(i * 16, k * 16, j * 16));
                loadList.push_back(newChunk);
            }
        }
    }
}

ChunkManager::~ChunkManager() {
    for (size_t i = 0; i < loadList.size(); i++)
    {
        delete loadList[i];
    }
    
}

void ChunkManager::LoadChunk() {
    (void)renderer;
    Chunk *nextToLoad = NULL;
    for (size_t i = 0; i < loadList.size(); i++) {
        if (loadList[i]->loaded == false) {
            nextToLoad = loadList[i];
        }
    }
    if (nextToLoad)
        nextToLoad->CreateMesh();
}