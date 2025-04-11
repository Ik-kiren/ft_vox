#include "../includes/ChunkManager.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/Renderer.hpp"
#include <chrono>

using namespace std::chrono_literals;

ChunkManager::ChunkManager(Renderer *renderer): renderer(renderer) {
    (void)camera;
    this->maxPos = Vector3(15, -1, 15);
    this->minPos = Vector3(-15, -16, -15);
    for (int i = this->minPos.x; i <= this->maxPos.x; i++)
    {
        for (int j = this->minPos.y; j <= this->maxPos.y; j++)
        {
            for (int k = this->minPos.z; k <= this->maxPos.z; k++) {
                Chunk *newChunk = new Chunk(renderer, this);
                if (i == 0 && j == -1 && k == 0) {
                    newChunk->GetBlocksArray()[0][15][0].SetActive(false);
                    newChunk->GetBlocksArray()[0][15][1].SetActive(false);
                    newChunk->GetBlocksArray()[1][15][0].SetActive(false);
                    newChunk->GetBlocksArray()[0][14][0].SetActive(false);
                    newChunk->GetBlocksArray()[0][13][0].SetActive(false);
                }
                newChunk->Translation(Vector3(i * Chunk::CHUNK_SIZE_X, j * Chunk::CHUNK_SIZE_Y, k * Chunk::CHUNK_SIZE_Z));
                newChunk->chunkList = &(this->chunkList);
                loadList.push_back(newChunk);
                chunkList.push_back(newChunk);
                chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
            }
        }
    }
    this->threading = false;
    std::cout << loadList.size() << std::endl;
}

ChunkManager::~ChunkManager() {
    for (size_t i = 0; i < chunkList.size(); i++)
    {
        delete chunkList[i];
    }
}

void ChunkManager::Init() {
    while (loadList.size() > 0 || futureList.size() > 0) {
        LoadChunk();
    }
}

Chunk *ChunkManager::LoadThread(Chunk *chunk) {
    chunk->CreateMesh();
    return chunk;
}

void ChunkManager::LoadChunk() {
    int tmp = 0;
    for (std::vector<Chunk *>::iterator it = loadList.begin(); it != loadList.end();) {
        if ((*it)->loaded == false) {
            threadList.push_back(*it);
            it = loadList.erase(it);
            /*tmp++;
            if (tmp >= 4)
                break;*/
        } else {
            it++;
        }
    }
    tmp = 0;
    if (threadList.size() == 0) {
        tmp = 0;
    } else {
        tmp = futureList.size();
    }
    
    for (std::vector<Chunk *>::iterator it = threadList.begin(); it != threadList.end();) {
        //futureList.push_back(std::async(&ChunkManager::LoadThread, this, (*it)));
        (*it)->CreateMesh();
        setupList.push_back((*it));
        it = threadList.erase(it);
        /*tmp++;
        if (tmp >= 4)
            break;*/
    }
    /*if (futureList.size() > 0) {
        for (std::vector<std::future<Chunk *>>::iterator it = futureList.begin(); it != futureList.end(); it++) {
            if ((*it).wait_for(0s) == std::future_status::ready) {
                setupList.push_back((*it).get());
                if (futureList.erase(it) == futureList.end()) {
                    break;
                }
            }
        }
        
    }*/
    //std::cout << futureList.size() << std::endl;
}

void ChunkManager::ChunkSetup() {
    for (std::vector<Chunk *>::iterator it = setupList.begin(); it != setupList.end(); it++) {
        if (!(*it)->meshed) {
            renderer->FinishMesh((*it)->meshID);
            (*it)->meshed = true;
        }
        visibilityList.push_back((*it));
        if (setupList.erase(it) == setupList.end())
            break;
    }
}

void ChunkManager::ChunkVisibility(Camera *camera) {
    /*renderList.clear();
    for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end(); it++) {
        AABB aabb;
        aabb.center[0] = (*it)->GetPosition().x + 8;
        aabb.center[1] = (*it)->GetPosition().y + 8;
        aabb.center[2] = (*it)->GetPosition().z + 8;
        if (camera->InsideFrustum(aabb)) {
            renderList.push_back(*it);
        }
    }*/
    if (visibilityList.size() > 0)
        renderer->Render(visibilityList);
    //lastCamPos = camera->GetPosition();
}