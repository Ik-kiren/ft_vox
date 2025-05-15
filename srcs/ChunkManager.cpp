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
				player->setYfromChunk(this->_chunk);
            if (i == this->maxPos.x || j == this->maxPos.z || i == this->minPos.x || j == this->minPos.z)
                this->AddTrailChunk(i + player->getPos().x / 16, j + player->getPos().z / 16);
            else
			    this->loadNewChunk(i + player->getPos().x / 16, j + player->getPos().z / 16);
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
    for (std::vector<Chunk *>::iterator it = loadList.begin(); it != loadList.end();) {
        if ((*it)->unload) {
            if (unloadMap.find((*it)->GetNormalizedPos()) == unloadMap.end())
                unloadMap.insert({(*it)->GetNormalizedPos(), (*it)});
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
        } else if ((*it)->update) {
            (*it)->UpdateMesh();
            setupList.push_back(*it);
            it = loadList.erase(it);
        } else {
            it++;
        }
    }
}

void ChunkManager::ChunkSetup() {
    for (std::vector<Chunk *>::iterator it = setupList.begin(); it != setupList.end();) {
        if ((*it)->unload) {
            if (unloadMap.find((*it)->GetNormalizedPos()) == unloadMap.end())
                unloadMap.insert({(*it)->GetNormalizedPos(), (*it)});
            it = setupList.erase(it);
            continue;
        }
        if (!(*it)->meshed) {
            renderer->FinishMesh((*it)->meshID);
            (*it)->meshed = true;
        } else if ((*it)->update) {
            renderer->UpdateMesh((*it)->meshID);
            (*it)->update = false;
            it = setupList.erase(it);
            continue;
        }
        visibilityList.push_back((*it));
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
        for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
            if ((*it)->unload) {
                if (unloadMap.find((*it)->GetNormalizedPos()) == unloadMap.end())
                    unloadMap.insert({(*it)->GetNormalizedPos(), (*it)});
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
        if (chunkMap.find(Vector3(xdiff, i, zdiff)) != chunkMap.end()) {
            chunkMap[Vector3(xdiff, i, zdiff)]->unload = false;
            if (chunkMap[Vector3(xdiff, i, zdiff)]->loaded) {
                Vector3 vec = Vector3(xdiff, i, zdiff);
                visibilityList.push_back(chunkMap[Vector3(xdiff, i, zdiff)]);
            } else {
                loadList.push_back(chunkMap[Vector3(xdiff, i, zdiff)]);
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
		loadList.push_back(newChunk);
		chunkMap.insert({newChunk->GetNormalizedPos(), newChunk});
	}
}

void    ChunkManager::UpdateChunk(int xdiff, int zdiff) {
    for (int k = 0; k < 16; k++) {
        Vector3 pos = Vector3(xdiff, k, zdiff);
        if (chunkMap.find(pos) != chunkMap.end()) {
            chunkMap[pos]->update = true;
            chunkMap[pos]->unload = false;
            loadList.push_back(chunkMap[pos]);
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
    for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
        if ((*it)->GetNormalizedPos().x == xdiff) {
            it = visibilityList.erase(it);
        } else {
            it++;
        }
    }
    for (std::vector<Chunk *>::iterator it = setupList.begin(); it != setupList.end();) {
        if ((*it)->GetNormalizedPos().x == xdiff) {
            it = setupList.erase(it);
        } else {
            it++;
        }
    }
    for (std::unordered_map<Vector3, Chunk *>::iterator it = unloadMap.begin(); it != unloadMap.end();) {
        if (it->second->GetNormalizedPos().x == xdiff) {
            it = unloadMap.erase(it);
        } else {
            it++;
        }
    }
    for (std::vector<Chunk *>::iterator it = loadList.begin(); it != loadList.end();) {
        if ((*it)->GetNormalizedPos().x == xdiff) {
            it = loadList.erase(it);
        } else {
            it++;
        }
    }
}

void ChunkManager::deactivateChunkZ(int zdiff) {
    for (std::vector<Chunk *>::iterator it = visibilityList.begin(); it != visibilityList.end();) {
        if ((*it)->GetNormalizedPos().z == zdiff) {
            it = visibilityList.erase(it);
        } else {
            it++;
        }
    }
    for (std::vector<Chunk *>::iterator it = loadList.begin(); it != loadList.end();) {
        if ((*it)->GetNormalizedPos().z == zdiff) {
            it = loadList.erase(it);
        } else {
            it++;
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
	chunk *pos = this->tab->chunkToRet(newx, z);
	player->setChunk(pos);
    this->UnloadChunkX(oldx - RENDERSIZE * signe((int)(newx) - oldx));
}

void	ChunkManager::loadNewColumn(int oldz, int newz, int x, Player *player) {
	for (int j = this->minPos.x; j <= this->maxPos.x; j++) {
		this->tab->chunkToRet(x + j, oldz + RENDERSIZE * signe((int)(newz) - oldz) + signe((int)(newz) - oldz), this->_chunk);
        if (j != this->minPos.x && j != this->maxPos.x)
            this->GetLimitChunk(x + j, oldz + (RENDERSIZE - 1) * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));
        this->AddTrailChunk(x + j, oldz + RENDERSIZE * signe((int)(newz) - oldz) + signe((int)(newz) - oldz));
	}
    this->deactivateChunkZ(oldz - (RENDERSIZE - 1) * signe((int)(newz) - oldz));
	chunk *pos = this->tab->chunkToRet(x, newz);
	player->setChunk(pos);
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

