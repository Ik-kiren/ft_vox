#include "../includes/ChunkManager.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/Renderer.hpp"
#include <chrono>
#include <array>
#include <sys/time.h>

ChunkManager::ChunkManager(Renderer *renderer, mapGP *tab, Player *player): renderer(renderer) {
    camera = NULL;
    this->player = player;
    this->maxPos = Vector3(RENDERSIZE, 15, RENDERSIZE);
    this->minPos = Vector3(-RENDERSIZE, 0, -RENDERSIZE);
	this->tab = tab;

	this->_chunk = new unsigned char***[16];
	for (int i = 0; i < 16; i++) {
		this->_chunk[i] = new unsigned char**[16];
		for (int j = 0; j < 16; j++) {
			this->_chunk[i][j] = new unsigned char*[16];
			for (int k = 0; k < 16; k++) {
				this->_chunk[i][j][k] = new unsigned char[16];
				for (int l = 0; l < 16; l++)
					this->_chunk[i][j][k][l] = 0;
			}
		}
	}

 	for (int i = this->minPos.x; i <= this->maxPos.x; i++) {
		for (int j = this->minPos.z ; j <= this->maxPos.z; j++) {
			tab->chunkToRet(i + player->getPos().x / 16, j + player->getPos().z / 16, this->_chunk);
			if (i == 0 && j == 0)
				player->setChunk(this->_chunk);
            if (i == this->maxPos.x || j == this->maxPos.z || i == this->minPos.x || j == this->minPos.z)
                this->AddTrailChunk(i + player->getPos().x / 16, j + player->getPos().z / 16);
            else
			    this->loadNewChunk(i + player->getPos().x / 16, j + player->getPos().z / 16);
		}
	}
}

ChunkManager::~ChunkManager() {
    for (std::unordered_map<Vector3, Chunk *>::iterator it = loadList.begin(); it != loadList.end();) {
        if (chunkMap.find(it->second->GetNormalizedPos()) != chunkMap.end())
            chunkMap.erase(chunkMap.find(it->second->GetNormalizedPos()));
        delete it->second;
        it = loadList.erase(it);
    }
    for (std::unordered_map<Vector3, Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
        if (chunkMap.find(it->second->GetNormalizedPos()) != chunkMap.end())
            chunkMap.erase(chunkMap.find(it->second->GetNormalizedPos()));
        delete it->second;
        it = visibilityList.erase(it);
    }
    for (std::unordered_map<Vector3, Chunk *>::iterator it = setupList.begin(); it != setupList.end();) {
        if (chunkMap.find(it->second->GetNormalizedPos()) != chunkMap.end())
            chunkMap.erase(chunkMap.find(it->second->GetNormalizedPos()));
        delete it->second;
        it = setupList.erase(it);
    }
    ChunkUnload();
    for (std::unordered_map<Vector3, Chunk*>::iterator it = chunkMap.begin(); it != chunkMap.end();) {
        delete it->second;
        it = chunkMap.erase(it);
    }
	freeChunksAll(this->_chunk);
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

    for (std::unordered_map<Vector3, Chunk *>::iterator it = loadList.begin(); it != loadList.end();) {
        if (it->second->unload) {
            if (unloadMap.find(it->second->GetNormalizedPos()) == unloadMap.end())
                unloadMap.insert({it->second->GetNormalizedPos(), it->second});
            it = loadList.erase(it);
            continue;
        }
        if (it->second->loaded == false) {	
            it->second->CreateMesh();
            setupList.insert({it->first, it->second});
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
    for (std::unordered_map<Vector3, Chunk *>::iterator it = setupList.begin(); it != setupList.end();) {
        if (it->second->unload) {
            if (unloadMap.find(it->second->GetNormalizedPos()) == unloadMap.end())
                unloadMap.insert({it->second->GetNormalizedPos(), it->second});
            it = setupList.erase(it);
            continue;
        }
        if (!it->second->meshed) {
            renderer->FinishMesh(it->second->meshID);
            it->second->meshed = true;
        }
        visibilityList.insert({it->first, it->second});
        it = setupList.erase(it);
    }
}

void ChunkManager::ChunkUnload() {
    for (std::unordered_map<Vector3, Chunk *>::iterator it = unloadMap.begin(); it != unloadMap.end();) {
        if (!it->second->unload) {
            it = unloadMap.erase(it);
            continue;
        }
        if (chunkMap.find(it->first) != chunkMap.end())
            chunkMap.erase(chunkMap.find(it->first));
        delete it->second;
        it = unloadMap.erase(it);
    }
}

void ChunkManager::ChunkVisibility() {
    if (lastCamPos != camera->GetPosition() || lastCamDirection != camera->GetFront()) {
        renderList.clear();

        for (std::unordered_map<Vector3, Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
            if (it->second->unload) {
                if (unloadMap.find(it->second->GetNormalizedPos()) == unloadMap.end())
                    unloadMap.insert({it->second->GetNormalizedPos(), it->second});
                it = visibilityList.erase(it);
                continue;
            }
            AABB aabb;
            aabb.center[0] = it->second->GetPosition().x + 8.5f;
            aabb.center[1] = it->second->GetPosition().y + 8.5f;
            aabb.center[2] = it->second->GetPosition().z + 8.5f;
            if (camera->InsideFrustum(aabb)) {
                renderList.push_back(it->second);
            }
            it++;
        }
    }
    if (renderList.size() > 0)
        renderer->Render(renderList);
    lastCamPos = camera->GetPosition();
    lastCamDirection = camera->GetFront();
}

void ChunkManager::UnloadChunkX(int x) {
    for (int i = this->minPos.y; i <= this->maxPos.y; i++) {
        for (int j = GetMinChunkPos().z - 3; j <= GetMaxChunkPos().z + 3; j++) {
            if (chunkMap.find(Vector3(x, i, j)) != chunkMap.end()) {
                // chunkMap[Vector3(x, i, j)]->unload = true;
				delete chunkMap[Vector3(x, i, j)];
				chunkMap.erase(Vector3(x, i, j));
            }
        }
    }
}

void ChunkManager::UnloadChunkZ(int z) {
    for (int i = GetMinChunkPos().x - 3; i <= GetMaxChunkPos().x + 3; i++) {
        for (int j = this->minPos.y; j <= this->maxPos.y; j++) {
			if (chunkMap.find(Vector3(i, j , z)) != chunkMap.end()) {
                // chunkMap[Vector3(i, j, z)]->unload = true;
				delete chunkMap[Vector3(i, j, z)];
				chunkMap.erase(Vector3(i, j, z));
            }
        }
    }
}

void ChunkManager::AddTrailChunk(int xdiff, int zdiff) {
    for (int i = 0; i < 16; i++) {
        if (chunkMap.find(Vector3(xdiff, i, zdiff)) == chunkMap.end()) {
            Chunk *newChunk = new Chunk(this->renderer, this, this->_chunk[i]);
		    newChunk->Translation(Vector3(xdiff * Chunk::CHUNK_SIZE_X, i * Chunk::CHUNK_SIZE_Y, zdiff * Chunk::CHUNK_SIZE_Z));
			chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
        }
    }
}

void ChunkManager::GetLimitChunk(int xdiff, int zdiff) {
    for (int i = 0; i < 16; i++) {
        Vector3 vec = Vector3(xdiff, i, zdiff);
        if (chunkMap.find(vec) != chunkMap.end()) {
            chunkMap[vec]->unload = false;
            if (chunkMap[vec]->loaded) {
                visibilityList.insert({vec, chunkMap[vec]});
            } else {
                loadList.insert({vec, chunkMap[vec]});
            }
        } else {
            std::cout << "Error: limit chunk not found in chunkMap" << std::endl;
        }
    }
}

void	ChunkManager::loadNewChunk(int xdiff, int zdiff) {
	for (int k = 0; k < 16; k++) {
		Chunk *newChunk = new Chunk(this->renderer, this, this->_chunk[k]);
		newChunk->Translation(Vector3(xdiff * Chunk::CHUNK_SIZE_X, k * Chunk::CHUNK_SIZE_Y, zdiff * Chunk::CHUNK_SIZE_Z));
        loadList.insert({newChunk->GetNormalizedPos(), newChunk});
		chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
	}
}

void    ChunkManager::UpdateChunk(int xdiff, int zdiff) {
    for (int k = 0; k < 16; k++) {
        Vector3 pos = Vector3(xdiff, k, zdiff);
        if (chunkMap.find(pos) != chunkMap.end()) {
            chunkMap[pos]->update = true;
            chunkMap[pos]->unload = false;
            loadList[pos] = chunkMap[pos];
        }
	}
}

void ChunkManager::ChunkManagerLoop() {
    ChunkVisibility();
    LoadChunk();
    ChunkSetup();
    ChunkUnload();
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

void ChunkManager::deactivateChunkX(int xdiff) {
    for (int i = GetMinChunkPos().z - 2; i <= GetMaxChunkPos().z + 2; i++) {
        for (int j = minPos.y; j <= maxPos.y; j++) {
            Vector3 vecToDeactivate = Vector3(xdiff, j, i);
            loadList.erase(vecToDeactivate);
            visibilityList.erase(vecToDeactivate);
            setupList.erase(vecToDeactivate);
            unloadMap.erase(vecToDeactivate);
        }
    }
}

void ChunkManager::deactivateChunkZ(int zdiff) {
    for (int i = GetMinChunkPos().x - 2; i <= GetMaxChunkPos().x + 2; i++) {
        for (int j = minPos.y; j <= maxPos.y; j++) {
            Vector3 vecToDeactivate = Vector3(i, j, zdiff);
            loadList.erase(vecToDeactivate);
            visibilityList.erase(vecToDeactivate);
            setupList.erase(vecToDeactivate);
            unloadMap.erase(vecToDeactivate);
        }
    }
}

void	ChunkManager::loadNewLine(int oldx, int newx, int z, Player *player) {
	for (int j = this->minPos.z; j <= this->maxPos.z; j++) {
		this->tab->chunkToRet(oldx + RENDERSIZE * signe((int)(newx) - oldx) + signe((int)(newx) - oldx), z + j, this->_chunk);
        if (j != this->minPos.z && j != this->maxPos.z)
            this->GetLimitChunk(oldx + (RENDERSIZE - 1) * signe((int)(newx) - oldx) + signe((int)(newx) - oldx), z + j);
        this->AddTrailChunk(oldx + RENDERSIZE * signe((int)(newx) - oldx) + signe((int)(newx) - oldx), z + j);
	}
    this->deactivateChunkX(oldx - (RENDERSIZE - 1) * signe((int)(newx) - oldx));
	this->tab->chunkToRet(newx, z, this->_chunk);
	player->setChunk(this->_chunk);
    this->UnloadChunkX(oldx - RENDERSIZE * signe((int)(newx) - oldx));
}

void	ChunkManager::loadNewColumn(int oldz, int newz, int x, Player *player) {
	int	moy = 0;
	for (int j = this->minPos.x; j <= this->maxPos.x; j++) {
		struct timeval tp0;
		gettimeofday(&tp0, NULL);

		this->tab->chunkToRet(x + j, oldz + RENDERSIZE * signe((int)(newz) - oldz) + signe((int)(newz) - oldz), this->_chunk);
		
		struct timeval tp1;
		gettimeofday(&tp1, NULL);
		moy += tp1.tv_usec - tp0.tv_usec;
		std::cout << tp1.tv_sec - tp0.tv_sec << " sec " << tp1.tv_usec - tp0.tv_usec << " ms\n";

        if (j != this->minPos.x && j != this->maxPos.x)
            this->GetLimitChunk(x + j, oldz + (RENDERSIZE - 1) * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));
			
        this->AddTrailChunk(x + j, oldz + RENDERSIZE * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));


	}
	std::cout << moy << '\n';
    this->deactivateChunkZ(oldz - (RENDERSIZE - 1) * signe((int)(newz) - oldz));
	this->tab->chunkToRet(x, newz, this->_chunk);
	player->setChunk(this->_chunk);
    this->UnloadChunkZ(oldz - RENDERSIZE * signe((int)(newz) - oldz));
}

void	ChunkManager::SetCamera(Camera *camera) {
    this->camera = camera;
}

void	ChunkManager::deleteCube(Camera &camera) {
	int i = camera.GetPosition().x / 16 - signeN(camera.GetPosition().x);
	int y = camera.GetPosition().y;
	int z = camera.GetPosition().z / 16 - signeN(camera.GetPosition().z);

	for (int j = 0; j < 16; j++) {
		if (chunkMap.find(Vector3(i, j, z)) != chunkMap.end()) {
			chunkMap[Vector3(i, j, z)]->unload = true;
			chunkMap.erase(Vector3(i, j, z));
		}
	}

	this->tab->deleteCube(camera.GetPosition().x, camera.GetPosition().z, y);

	this->tab->chunkToRet(i, z, this->_chunk);
	this->loadNewChunk(i, z);
}

