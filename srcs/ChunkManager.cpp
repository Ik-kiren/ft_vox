#include "../includes/ChunkManager.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/Renderer.hpp"
#include <chrono>
#include <array>

using namespace std::chrono_literals;

ChunkManager::ChunkManager(Renderer *renderer, chunk ***tab): renderer(renderer) {
    (void)camera;
    this->maxPos = Vector3(15, 15, 15);
    this->minPos = Vector3(0, 0, 0);
    for (int i = this->minPos.x; i <= this->maxPos.x; i++)
    {
        for (int j = this->minPos.y; j <= this->maxPos.y; j++)
        {
            for (int k = this->minPos.z; k <= this->maxPos.z; k++) {
                Chunk *newChunk = new Chunk(renderer, this, tab[i][k][j].voxel);
                newChunk->Translation(Vector3(i * Chunk::CHUNK_SIZE_X, j * Chunk::CHUNK_SIZE_Y, k * Chunk::CHUNK_SIZE_Z));
                newChunk->chunkList = &(this->chunkList);
                loadList.push_back(newChunk);
                chunkList.push_back(newChunk);
                chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
            }
        }
    }
    freeChunks(tab);
    std::cout << loadList.size() << std::endl;
}

ChunkManager::~ChunkManager() {
    for (size_t i = 0; i < chunkList.size(); i++)
    {
        delete chunkList[i];
    }
}

void ChunkManager::Init() {
    while (loadList.size() > 0) {
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
            (*it)->CreateMesh();
            setupList.push_back(*it);
            it = loadList.erase(it);
        } else {
            it++;
        }
    }
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
    if (lastCamPos != camera->GetPosition() || lastCamDirection != camera->GetFront()) {
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
    }
    if (renderList.size() > 0)
        renderer->Render(renderList);
    lastCamPos = camera->GetPosition();
    lastCamDirection = camera->GetFront();
}

void ChunkManager::UnloadChunk(Vector3 direction, Vector3 position) {
    if (direction == Vector3(1, 0, 0)) {
        for (int i = minPos.z; i < maxPos.z; i++) {
            for (int j = minPos.y; j < maxPos.y; j++) {
                Vector3 tmp = Vector3((abs(minPos.x) + abs(maxPos.x)) - position.x, i, j);
                std:: cout << tmp << std::endl;
            }
        }
    }
}

void	ChunkManager::loadNewChunk(chunk ***toLoad, int xdiff, int zdiff) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                Chunk *newChunk = new Chunk(this->renderer, this, toLoad[i][k][j].voxel);
                newChunk->Translation(Vector3((i + xdiff) * Chunk::CHUNK_SIZE_X, j * Chunk::CHUNK_SIZE_Y, (k + zdiff) * Chunk::CHUNK_SIZE_Z));
				newChunk->chunkList = &(this->chunkList);
				loadList.push_back(newChunk);
                chunkList.push_back(newChunk);
				chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
			}
		}
	}
    freeChunks(toLoad);
}