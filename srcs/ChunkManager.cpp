#include "../includes/ChunkManager.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/Renderer.hpp"
#include <chrono>
#include <array>

using namespace std::chrono_literals;

ChunkManager::ChunkManager(Renderer *renderer, mapGP *tab, Player *player): renderer(renderer) {
    camera = NULL;
    this->player = player;
    this->maxPos = Vector3(RENDERSIZE, 15, RENDERSIZE);
    this->minPos = Vector3(-RENDERSIZE, 0, -RENDERSIZE);
	this->tab = tab;

 	for (int i = this->minPos.x; i <= this->maxPos.x; i++) {
		for (int j = this->minPos.z; j <= this->maxPos.z; j++) {
			chunk *monoCx0 = tab->chunkToRet(i + player->getPos().x / 16, j + player->getPos().z / 16);
			if (i == 0 && j == 0)
				player->setYfromChunk(monoCx0);
			this->loadNewChunk(monoCx0, i + player->getPos().x / 16, j + player->getPos().z / 16);
		}
	}
}

ChunkManager::~ChunkManager() {
    for (std::vector<Chunk *>::iterator it = loadList.begin(); it != loadList.end();) {
        if (chunkMap.find((*it)->GetNormalizedPos()) != chunkMap.end())
            chunkMap.erase(chunkMap.find((*it)->GetNormalizedPos()));
        delete *it;
        it = loadList.erase(it);
    }
    for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
        if (chunkMap.find((*it)->GetNormalizedPos()) != chunkMap.end())
            chunkMap.erase(chunkMap.find((*it)->GetNormalizedPos()));
        delete *it;
        it = visibilityList.erase(it);
    }
    for (std::vector<Chunk *>::iterator it = setupList.begin(); it != setupList.end();) {
        if (chunkMap.find((*it)->GetNormalizedPos()) != chunkMap.end())
            chunkMap.erase(chunkMap.find((*it)->GetNormalizedPos()));
        delete *it;
        it = setupList.erase(it);
    }
    ChunkUnload();
    for (std::unordered_map<Vector3, Chunk*>::iterator it = chunkMap.begin(); it != chunkMap.end();) {
        delete it->second;
        it = chunkMap.erase(it);
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
        if ((*it)->unload) {
            unloadList.push_back(*it);
            it = loadList.erase(it);
            continue;
        }
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
    for (std::vector<Chunk *>::iterator it = setupList.begin(); it != setupList.end();) {
        if ((*it)->unload) {
            unloadList.push_back(*it);
            it = setupList.erase(it);
            continue;
        }
        if (!(*it)->meshed) {
            renderer->FinishMesh((*it)->meshID);
            (*it)->meshed = true;
        }
        visibilityList.push_back((*it));
        it = setupList.erase(it);
    }
}

void ChunkManager::ChunkUnload() {
    for (std::vector<Chunk *>::iterator it = unloadList.begin(); it != unloadList.end();) {
        if (chunkMap.find((*it)->GetNormalizedPos()) != chunkMap.end())
            chunkMap.erase(chunkMap.find((*it)->GetNormalizedPos()));
        delete *it;
        it = unloadList.erase(it);
    }
}

void ChunkManager::ChunkVisibility() {
    if (lastCamPos != camera->GetPosition() || lastCamDirection != camera->GetFront()) {
        renderList.clear();
        for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end(); it++) {
            if ((*it)->unload) {
                unloadList.push_back(*it);
                it = visibilityList.erase(it);
                continue;
            }
            AABB aabb;
            aabb.center[0] = (*it)->GetPosition().x + 8.5f;
            aabb.center[1] = (*it)->GetPosition().y + 8.5f;
            aabb.center[2] = (*it)->GetPosition().z + 8.5f;
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
    for (int i = GetMinChunkPos().y; i <= GetMaxChunkPos().y; i++) {
        for (int j = GetMinChunkPos().z - 3; j <= GetMaxChunkPos().z + 3; j++) {
            if (chunkMap.find(Vector3(x, i , j)) != chunkMap.end()) {
                chunkMap[Vector3(x, i, j)]->unload = true;
                chunkMap.erase(Vector3(x, i , j));
            }
        }
    }
}

void ChunkManager::UnloadChunkZ(int z) {
    for (int i = GetMinChunkPos().x - 3; i <= GetMaxChunkPos().x + 3; i++) {
        for (int j = GetMinChunkPos().y; j <= GetMaxChunkPos().y; j++) {
			if (chunkMap.find(Vector3(i, j , z)) != chunkMap.end()) {
                chunkMap[Vector3(i, j, z)]->unload = true;
                chunkMap.erase(Vector3(i, j , z));
            }
        }
    }
}

void	ChunkManager::loadNewChunk(chunk *toLoad, int xdiff, int zdiff) {

	for (int k = 0; k < 16; k++) {
		Chunk *newChunk = new Chunk(this->renderer, this, toLoad[k].voxel);
		newChunk->Translation(Vector3(xdiff * Chunk::CHUNK_SIZE_X, k * Chunk::CHUNK_SIZE_Y, zdiff * Chunk::CHUNK_SIZE_Z));
		loadList.push_back(newChunk);
		chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
	}
    freeChunks(toLoad);
}

void ChunkManager::ChunkManagerLoop() {
    LoadChunk();
    ChunkSetup();
    ChunkUnload();
    ChunkVisibility();
}

Vector3 ChunkManager::GetMaxChunkPos() {
    Vector3 tmp(0.0);
    if (camera != NULL) {
        tmp = camera->GetPosition();
    } else {
        tmp = player->getPos();
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
        tmp = player->getPos();
    }
    if (tmp.x < 0)
        tmp.x -= 16;
    if (tmp.z < 0)
        tmp.z -= 16;
    tmp = (tmp / 16).Trunc() - ((maxPos - minPos) / 2);
    return tmp.Trunc();
}

void	ChunkManager::loadNewLine(int oldx, int newx, int z) {
	for (int j = this->minPos.z; j <= this->maxPos.z; j++) {
		chunk *monoCx1 = this->tab->chunkToRet(oldx + RENDERSIZE * signe((int)(newx) - oldx) + signe((int)(newx) - oldx), z + j);
		this->loadNewChunk(monoCx1, oldx + RENDERSIZE * signe((int)(newx) - oldx) + signe((int)(newx) - oldx), z + j);
	}
	this->UnloadChunkX(oldx - RENDERSIZE * signe((int)(newx) - oldx));
}

void	ChunkManager::loadNewColumn(int oldz, int newz, int x) {
	for (int j = this->minPos.x; j <= this->maxPos.x; j++) {
		chunk *monoCx2 = this->tab->chunkToRet(x + j, oldz + RENDERSIZE * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));
		this->loadNewChunk(monoCx2, x + j, oldz + RENDERSIZE * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));
	}
	this->UnloadChunkZ(oldz - RENDERSIZE * signe((int)(newz) - oldz));
}

void ChunkManager::SetCamera(Camera *camera) {
    this->camera = camera;
}