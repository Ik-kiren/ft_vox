#include "../includes/ChunkManager.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/Renderer.hpp"
#include <chrono>

using namespace std::chrono_literals;

ChunkManager::ChunkManager(Renderer *renderer, Camera *camera): renderer(renderer), camera(camera) {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 8; k++) {
                Chunk *newChunk = new Chunk(renderer);
                newChunk->Translation(Vector3(i * 16, j * -16, k * 16));
                loadList.push_back(newChunk);
            }
        }
    }
    this->threading = false;
    std::cout << loadList.size() << std::endl;
}

ChunkManager::~ChunkManager() {
    for (size_t i = 0; i < loadList.size(); i++)
    {
        delete loadList[i];
    }

    for (size_t i = 0; i < threadList.size(); i++)
    {
        delete threadList[i];
    }

    for (size_t i = 0; i < setupList.size(); i++)
    {
        delete setupList[i];
    }

    for (size_t i = 0; i < visibilityList.size(); i++)
    {
        delete visibilityList[i];
    }
}

Chunk *ChunkManager::LoadThread(Chunk *chunk) {
    chunk->CreateMesh();
    return chunk;
}

void ChunkManager::LoadChunk() {
    int tmp = 0;
    for (std::vector<Chunk *>::iterator it = loadList.begin(); it != loadList.end(); it++) {
        if ((*it)->loaded == false) {
            threadList.push_back(*it);
            if (loadList.erase(it) == loadList.end()) {
                break;
            }
            tmp++;
            if (tmp >= 32)
                break;
        }
    }
    tmp = 0;
    if (futureList.size() < 32) {
        if (threadList.size() == 0) {
            tmp = 0;
        } else {
            tmp = futureList.size();
        }
        for (std::vector<Chunk *>::iterator it = threadList.begin(); it != threadList.end(); it++) {
            futureList.push_back(std::async(&ChunkManager::LoadThread, this, (*it)));
            if (threadList.erase(it) == threadList.end()) {
                break;
            }
            tmp++;
            if (tmp >= 32)
                break;
        }
    }
    if (futureList.size() > 0) {
        for (std::vector<std::future<Chunk *>>::iterator it = futureList.begin(); it != futureList.end(); it++) {
            if ((*it).wait_for(0s) == std::future_status::ready) {
                setupList.push_back((*it).get());
                if (futureList.erase(it) == futureList.end()) {
                    break;
                }
            }
        }
        
    }
    std::cout << futureList.size() << std::endl;
    tmp = 0;
    for (std::vector<Chunk *>::iterator it = setupList.begin(); it != setupList.end(); it++) {
        if (!(*it)->meshed) {
            renderer->FinishMesh((*it)->meshID);
            (*it)->meshed = true;
        }
        visibilityList.push_back((*it));
        if (setupList.erase(it) == setupList.end())
            break;
        tmp++;
        if (tmp >= 32) {
            break;
        }
    }
}

void ChunkManager::ChunkVisibility() {
    renderList.clear();
    for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end(); it++) {
        AABB aabb;
        aabb.center[0] = (*it)->GetPosition().x + 8;
        aabb.center[1] = (*it)->GetPosition().y + 8;
        aabb.center[2] = (*it)->GetPosition().z + 8;
        if (camera->InsideFrustum(aabb)) {
            renderList.push_back(*it);
        }
    }
    if (renderList.size() > 0)
        renderer->Render(renderList);
    //lastCamPos = camera->GetPosition();
}