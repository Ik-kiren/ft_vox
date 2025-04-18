#include "../includes/ChunkManager.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/Renderer.hpp"
#include <chrono>
#include <array>

using namespace std::chrono_literals;

ChunkManager::ChunkManager(Renderer *renderer, mapGP &tab): renderer(renderer) {
    camera = NULL;
    this->maxPos = Vector3(1, 15, 1);
    this->minPos = Vector3(-1, 0, -1);
	this->tab = tab;

 	for (int i = this->minPos.x; i <= this->maxPos.x; i++) {
		for (int j = this->minPos.z; j <= this->maxPos.z; j++) {
			chunk *monoCx0 = tab.chunkToRet(i, j);
			this->loadNewChunk(monoCx0, i, j);
		}
	}
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
			tmp++;
			if (tmp == 8)
				break;
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

void ChunkManager::ChunkVisibility() {
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

void ChunkManager::UnloadChunkX(int x) {
    for (int i = minPos.y; i <= maxPos.y; i++) {
        for (int j = GetMinChunkPos().z; j <= GetMaxChunkPos().z; j++) {
            chunkMap.erase(Vector3(x, i , j));
        }
    }
    for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
        if ((*it)->GetNormalizedPos().x == x) {
            it = visibilityList.erase(it);
        }
        else {
            it++;
        }
    }
    for (int i = minPos.y; i <= maxPos.y; i++) {
        for (int j = minPos.z; j <= maxPos.z; j++) {
			// delete chunkMap[Vector3(x, i, j)];
            chunkMap.erase(Vector3(x, i , j));
        }
    }
}

void ChunkManager::UnloadChunkZ(int z) {
    for (int i = GetMinChunkPos().x; i <= GetMaxChunkPos().x; i++) {
        for (int j = minPos.y; j <= maxPos.y; j++) {
            chunkMap.erase(Vector3(i, j , z));
        }
    }
    for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
        if ((*it)->GetNormalizedPos().z == z) {
            it = visibilityList.erase(it);
        }
        else {
            it++;
        }
    }
    for (int i = minPos.x; i <= maxPos.x; i++) {
        for (int j = minPos.y; j <= maxPos.y; j++) {
			// delete chunkMap[Vector3(i, j, z)];
            chunkMap.erase(Vector3(i, j , z));
        }
    }
}

void	ChunkManager::loadNewChunk(chunk *toLoad, int xdiff, int zdiff) {
	// if (xdiff >= 0 && this->maxPos.x < xdiff)
	// 	this->maxPos = Vector3(xdiff, 15, this->maxPos.z);
	// if (zdiff >= 0 && this->maxPos.z < zdiff)
	// 	this->maxPos = Vector3(this->maxPos.x, 15, zdiff);
	// if (xdiff < 0 && this->minPos.x > xdiff)
	// 	this->minPos = Vector3(xdiff, 0, this->minPos.z);
	// if (zdiff < 0 && this->minPos.z > zdiff)
	// 	this->minPos = Vector3(this->minPos.x, 0, zdiff);

	for (int k = 0; k < 16; k++) {
		Chunk *newChunk = new Chunk(this->renderer, this, toLoad[k].voxel);
		newChunk->Translation(Vector3(xdiff * Chunk::CHUNK_SIZE_X, k * Chunk::CHUNK_SIZE_Y, zdiff * Chunk::CHUNK_SIZE_Z));
		newChunk->chunkList = &(this->chunkList);
		loadList.push_back(newChunk);
		chunkList.push_back(newChunk);
		chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
	}
    freeChunks(toLoad);
}

Vector3 ChunkManager::GetMaxChunkPos() {
    Vector3 tmp(0.0);
    if (camera != NULL) {
        tmp = camera->GetPosition();
    } else {
        tmp = Vector3(0, 128, 0);
    }
    if (tmp.x < 0)
        tmp.x -= 16;
    if (tmp.z < 0)
        tmp.z -= 16;
    tmp = (tmp / 16).Trunc() + ((maxPos - minPos) / 2);
    return tmp.Trunc();
}

Vector3 ChunkManager::GetMinChunkPos() {
    Vector3 tmp(0.0);
    if (camera != NULL) {
        tmp = camera->GetPosition();
    } else {
        tmp = Vector3(0, 128, 0);
    }
    if (tmp.x < 0)
        tmp.x -= 16;
    if (tmp.z < 0)
        tmp.z -= 16;
    tmp = (tmp / 16).Trunc() - ((maxPos - minPos) / 2);
    return tmp.Trunc();
}


void	ChunkManager::loadNewLine(int oldx, int newx, int z) {
	this->UnloadChunkX(oldx - 7 * signe((int)(newx) - oldx));
	for (int j = this->minPos.z; j <= this->maxPos.z; j++) {
		chunk *monoCx1 = this->tab.chunkToRet(oldx + 7 * signe((int)(newx) - oldx) + signe((int)(newx) - oldx), z + j);
		this->loadNewChunk(monoCx1, oldx + 7 * signe((int)(newx) - oldx) + signe((int)(newx) - oldx), z + j);
	}
}

void	ChunkManager::loadNewColumn(int oldz, int newz, int x) {
	this->UnloadChunkZ(oldz - 7 * signe((int)(newz) - oldz));
	for (int j = this->minPos.x; j <= this->maxPos.x; j++) {
		chunk *monoCx2 = this->tab.chunkToRet(x + j, oldz + 7 * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));
		this->loadNewChunk(monoCx2, x + j, oldz + 7 * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));
	}
}

void ChunkManager::SetCamera(Camera *camera) {
    this->camera = camera;
}