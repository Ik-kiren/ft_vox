#include "../includes/Chunk.hpp"
#include "../includes/Block.hpp"
#include "../includes/Vector3.hpp"
#include "../includes/Renderer.hpp"
#include "../includes/ChunkManager.hpp"

Chunk::Chunk(Renderer *renderer, ChunkManager *chunkManager, unsigned char ***test) : renderer(renderer), chunkManager(chunkManager) {
    this->loaded = false;
    this->activated = true;
    this->meshed = false;
    this->unload = false;
    this->update = false;
    this->meshID = 0;
    this->blocksArray = new Block**[CHUNK_SIZE_X];
    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        this->blocksArray[i] = new Block*[CHUNK_SIZE_Y];
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            this->blocksArray[i][j] = new Block[CHUNK_SIZE_Z];
            for (int k = 0; k < CHUNK_SIZE_Z; k++) {
                this->blocksArray[i][j][k].SetActive(test[i][k][j]);
                this->blocksArray[i][j][k].type = static_cast<BlockType>(test[i][k][j]);
            }
        }
    }
}

Chunk::~Chunk() {
    renderer->EraseMesh(meshID);
    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            delete[] this->blocksArray[i][j]; 
        }
        delete[] this->blocksArray[i];
    }
    delete[] this->blocksArray;
}

bool Chunk::CheckNextFace(Block &block, Block &nextBlock, Block &nextBlockUp, Block &nextChunkBlock, int up) {
    if (nextBlock.type != block.type || !nextBlock.IsActive() || nextBlock.visited[YPOSITIF] || (up + 1 < 16 && (nextBlockUp.IsActive()) && (nextBlockUp.type != ICE))
        || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, 1, 0)) != this->chunkManager->chunkMap.end() && up == 15 && (nextChunkBlock.IsActive() && nextChunkBlock.type != ICE)))
            return true;
    return false;
}


void Chunk::CreateCube(int &x, int &y, int &z, bool &xPositif, bool &xNegatif, bool &yPositif, bool &yNegatif, bool &zPositif, bool &zNegatif, int type) {
    unsigned int ui1 = 0;
    unsigned int ui2 = 0;
    unsigned int ui3 = 0;
    unsigned int ui4 = 0;
    unsigned int ui5 = 0;
    unsigned int ui6 = 0;
    unsigned int ui7 = 0;
    unsigned int ui8 = 0;
    Vector3 size(1);
    if (!yPositif) {
        size = Vector3(1);
        blocksArray[x][y][z].visited[YPOSITIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != blocksArray[x][y][z].type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[YPOSITIF] || (y + 1 < 16 && CheckIce(this->blocksArray[x][y + 1][j + 1], this->blocksArray[x][y][z])) || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, 1, 0)) != this->chunkManager->chunkMap.end() && y == 15 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, 1, 0)]->GetBlocksArray()[x][0][j + 1], blocksArray[x][y][z]))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[YPOSITIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[i + 1][y][k].type != blocksArray[x][y][z].type || !blocksArray[i + 1][y][k].IsActive() || blocksArray[i + 1][y][k].visited[YPOSITIF] || (y + 1 < 16 && CheckIce(blocksArray[i + 1][y + 1][k], blocksArray[x][y][z]))  || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, 1, 0)) != this->chunkManager->chunkMap.end() && y == 15 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, 1, 0)]->GetBlocksArray()[i + 1][0][k], blocksArray[x][y][z])))) {
                    is_row_good = false;
                    break;
                }
            }
            if (is_row_good) {
                size.x += 1;
                for (int k = z; k < z + size.z && k < 16; k++) {
                    blocksArray[i + 1][y][k].visited[YPOSITIF] = true;
                }
            } else {
                break;
            }
        }
        if (this->blocksArray[x][y][z].type == WATER) {
            ui5 = renderer->AddVertex2(meshID, x                             , y + Block::BLOCK_SIZE, z                          , type, Vector2(1, 1), YPOSITIF);
            ui1 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y + Block::BLOCK_SIZE, z                        , type, Vector2(size.x, 1), YPOSITIF);
            ui3 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y + Block::BLOCK_SIZE, z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.x, size.z), YPOSITIF);
            ui7 = renderer->AddVertex2(meshID, x                             , y + Block::BLOCK_SIZE, z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), YPOSITIF);
            renderer->addIndices2(meshID, ui1, ui5, ui7);
            renderer->addIndices2(meshID, ui1, ui7, ui3);
        } else {
            ui5 = renderer->AddVertex(meshID, x                             , y + Block::BLOCK_SIZE, z                          , type, Vector2(1, 1), YPOSITIF);
            ui1 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y + Block::BLOCK_SIZE, z                        , type, Vector2(size.x, 1), YPOSITIF);
            ui3 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y + Block::BLOCK_SIZE, z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.x, size.z), YPOSITIF);
            ui7 = renderer->AddVertex(meshID, x                             , y + Block::BLOCK_SIZE, z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), YPOSITIF);
            renderer->addIndices(meshID, ui1, ui5, ui7);
            renderer->addIndices(meshID, ui1, ui7, ui3);
        }
    }
    if (!zPositif) {
        size = Vector3(1);
        blocksArray[x][y][z].visited[ZPOSITIF] = true;
        for (int j = y; j + 1 < 16; j++) {
            if (blocksArray[x][j + 1][z].type != blocksArray[x][y][z].type || !blocksArray[x][j + 1][z].IsActive() || blocksArray[x][j + 1][z].visited[ZPOSITIF] || (z + 1 < 16 && (CheckIce(blocksArray[x][j + 1][z + 1], blocksArray[x][y][z])))  || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, 0, 1)) != this->chunkManager->chunkMap.end() && z == 15 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, 0, 1)]->GetBlocksArray()[x][j + 1][0], blocksArray[x][y][z]))))
                break;
            size.y += 1;
            blocksArray[x][j + 1][z].visited[ZPOSITIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = y; k < y + size.y && k < 16; k++) {
                if (blocksArray[i + 1][k][z].type != blocksArray[x][y][z].type || !blocksArray[i + 1][k][z].IsActive() || blocksArray[i + 1][k][z].visited[ZPOSITIF] || (z + 1 < 16 && (CheckIce(blocksArray[i + 1][k][z + 1], blocksArray[x][y][z])))  || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, 0, 1)) != this->chunkManager->chunkMap.end() && z == 15 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, 0, 1)]->GetBlocksArray()[i + 1][k][0], blocksArray[x][y][z])))) {
                    is_row_good = false;
                    break;
                }
            }
            if (is_row_good) {
                size.x += 1;
                for (int k = y; k < y + size.y && k < 16; k++) {
                    blocksArray[i + 1][k][z].visited[ZPOSITIF] = true;
                }
            } else {
                break;
            }
        }
        if (this->blocksArray[x][y][z].type == WATER) {
            ui8 = renderer->AddVertex2(meshID, x                               , y                               , z + Block::BLOCK_SIZE, type, Vector2(1, 1), ZPOSITIF);
            ui4 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y                               , z + Block::BLOCK_SIZE, type, Vector2(size.x, 1), ZPOSITIF);
            ui3 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y + (Block::BLOCK_SIZE * size.y), z + Block::BLOCK_SIZE, type, Vector2(size.x, size.y), ZPOSITIF);
            ui7 = renderer->AddVertex2(meshID, x                               , y + (Block::BLOCK_SIZE * size.y), z + Block::BLOCK_SIZE, type, Vector2(1, size.y), ZPOSITIF);
            
            renderer->addIndices2(meshID, ui4, ui3, ui7);
            renderer->addIndices2(meshID, ui4, ui7, ui8);
        } else {
            ui8 = renderer->AddVertex(meshID, x                               , y                               , z + Block::BLOCK_SIZE, type, Vector2(1, 1), ZPOSITIF);
            ui4 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y                               , z + Block::BLOCK_SIZE, type, Vector2(size.x, 1), ZPOSITIF);
            ui3 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y + (Block::BLOCK_SIZE * size.y), z + Block::BLOCK_SIZE, type, Vector2(size.x, size.y), ZPOSITIF);
            ui7 = renderer->AddVertex(meshID, x                               , y + (Block::BLOCK_SIZE * size.y), z + Block::BLOCK_SIZE, type, Vector2(1, size.y), ZPOSITIF);
            
            renderer->addIndices(meshID, ui4, ui3, ui7);
            renderer->addIndices(meshID, ui4, ui7, ui8);
        }
    }
    if (!xNegatif) {
        size = Vector3(1);
        blocksArray[x][y][z].visited[XNEGATIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != blocksArray[x][y][z].type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[XNEGATIF] || (x > 0 && (CheckIce(blocksArray[x - 1][y][j + 1], blocksArray[x][y][z])))  || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(-1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 0 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(-1, 0, 0)]->GetBlocksArray()[15][y][j + 1], blocksArray[x][y][z]))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[XNEGATIF] = true;
        }
        for (int i = y; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[x][i + 1][k].type != blocksArray[x][y][z].type || !blocksArray[x][i + 1][k].IsActive() || blocksArray[x][i + 1][k].visited[XNEGATIF] || (x > 0 && (CheckIce(blocksArray[x - 1][i + 1][k], blocksArray[x][y][z])))  || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(-1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 0 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(-1, 0, 0)]->GetBlocksArray()[15][i + 1][k], blocksArray[x][y][z])))) {
                    is_row_good = false;
                    break;
                }
            }
            if (is_row_good) {
                size.y += 1;
                for (int k = z; k < z + size.z && k < 16; k++) {
                    blocksArray[x][i + 1][k].visited[XNEGATIF] = true;
                }
            } else {
                break;
            }
        }
        if (this->blocksArray[x][y][z].type == WATER) {
            ui6 = renderer->AddVertex2(meshID, x                     , y                                 , z                               , type, Vector2(1, 1), XNEGATIF);
            ui5 = renderer->AddVertex2(meshID, x                     , y + (Block::BLOCK_SIZE * size.y)  , z                               , type, Vector2(size.y, 1), XNEGATIF);
            ui7 = renderer->AddVertex2(meshID, x                     , y + (Block::BLOCK_SIZE * size.y)  , z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.y, size.z), XNEGATIF);
            ui8 = renderer->AddVertex2(meshID, x                     , y                                 , z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), XNEGATIF);
            renderer->addIndices2(meshID, ui8, ui7, ui5);
            renderer->addIndices2(meshID, ui8, ui5, ui6);
        } else {
            ui6 = renderer->AddVertex(meshID, x                     , y                                 , z                               , type, Vector2(1, 1), XNEGATIF);
            ui5 = renderer->AddVertex(meshID, x                     , y + (Block::BLOCK_SIZE * size.y)  , z                               , type, Vector2(size.y, 1), XNEGATIF);
            ui7 = renderer->AddVertex(meshID, x                     , y + (Block::BLOCK_SIZE * size.y)  , z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.y, size.z), XNEGATIF);
            ui8 = renderer->AddVertex(meshID, x                     , y                                 , z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), XNEGATIF);
            renderer->addIndices(meshID, ui8, ui7, ui5);
            renderer->addIndices(meshID, ui8, ui5, ui6);
        }
    }
    if (!yNegatif) {
        size = Vector3(1);
        blocksArray[x][y][z].visited[YNEGATIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != blocksArray[x][y][z].type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[YNEGATIF] || (y > 0 && (CheckIce(blocksArray[x][y - 1][j + 1], blocksArray[x][y][z]))) || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, -1, 0)) != this->chunkManager->chunkMap.end() && y == 0 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, -1, 0)]->GetBlocksArray()[x][15][j + 1], blocksArray[x][y][z]))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[YNEGATIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[i + 1][y][k].type != blocksArray[x][y][z].type || !blocksArray[i + 1][y][k].IsActive() || blocksArray[i + 1][y][k].visited[YNEGATIF] || (y > 0 && (CheckIce(blocksArray[i + 1][y - 1][k], blocksArray[x][y][z]))) || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, -1, 0)) != this->chunkManager->chunkMap.end() && y == 0 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, -1, 0)]->GetBlocksArray()[i + 1][15][k], blocksArray[x][y][z])))) {
                    is_row_good = false;
                    break;
                }
            }
            if (is_row_good) {
                size.x += 1;
                for (int k = z; k < z + size.z && k < 16; k++) {
                    blocksArray[i + 1][y][k].visited[YNEGATIF] = true;
                }
            } else {
                break;
            }
        }
        if (this->blocksArray[x][y][z].type == WATER) {
            ui6 = renderer->AddVertex2(meshID, x                               , y                     , z                               , type, Vector2(1, 1), YNEGATIF);
            ui2 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y                     , z                               , type, Vector2(size.x, 1), YNEGATIF);
            ui4 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y                     , z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.x, size.z), YNEGATIF);
            ui8 = renderer->AddVertex2(meshID, x                               , y                     , z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), YNEGATIF);
            renderer->addIndices2(meshID, ui6, ui2, ui4);
            renderer->addIndices2(meshID, ui6, ui4, ui8);
        } else {
            ui6 = renderer->AddVertex(meshID, x                               , y                     , z                               , type, Vector2(1, 1), YNEGATIF);
            ui2 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y                     , z                               , type, Vector2(size.x, 1), YNEGATIF);
            ui4 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y                     , z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.x, size.z), YNEGATIF);
            ui8 = renderer->AddVertex(meshID, x                               , y                     , z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), YNEGATIF);
            renderer->addIndices(meshID, ui6, ui2, ui4);
            renderer->addIndices(meshID, ui6, ui4, ui8);
        }
    }
    if (!xPositif) {
        size = Vector3(1);
        blocksArray[x][y][z].visited[XPOSITIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != blocksArray[x][y][z].type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[XPOSITIF] || (x + 1 < 16 && (CheckIce(blocksArray[x + 1][y][j + 1], blocksArray[x][y][z]))) || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 15 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(1, 0, 0)]->GetBlocksArray()[0][y][j + 1], blocksArray[x][y][z]))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[XPOSITIF] = true;
        }
        for (int i = y; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[x][i + 1][k].type != blocksArray[x][y][z].type || !blocksArray[x][i + 1][k].IsActive() || blocksArray[x][i + 1][k].visited[XPOSITIF] || (x + 1 < 16 && (CheckIce(blocksArray[x + 1][i + 1][k], blocksArray[x][y][z]))) || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 15 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(1, 0, 0)]->GetBlocksArray()[0][i + 1][k], blocksArray[x][y][z])))) {
                    is_row_good = false;
                    break;
                }
            }
            if (is_row_good) {
                size.y += 1;
                for (int k = z; k < z + size.z && k < 16; k++) {
                    blocksArray[x][i + 1][k].visited[XPOSITIF] = true;
                }
            } else {
                break;
            }
        }
        if (this->blocksArray[x][y][z].type == WATER) {
            ui2 = renderer->AddVertex2(meshID, x + Block::BLOCK_SIZE, y                               , z                                , type, Vector2(1, 1), XPOSITIF);
            ui1 = renderer->AddVertex2(meshID, x + Block::BLOCK_SIZE, y + (Block::BLOCK_SIZE * size.y), z                                , type, Vector2(size.y, 1), XPOSITIF);
            ui3 = renderer->AddVertex2(meshID, x + Block::BLOCK_SIZE, y + (Block::BLOCK_SIZE * size.y), z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.y, size.z), XPOSITIF);
            ui4 = renderer->AddVertex2(meshID, x + Block::BLOCK_SIZE, y                               , z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), XPOSITIF);
            renderer->addIndices2(meshID, ui2, ui1, ui3);
            renderer->addIndices2(meshID, ui2, ui3, ui4);
        } else {
            ui2 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y                               , z                                , type, Vector2(1, 1), XPOSITIF);
            ui1 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + (Block::BLOCK_SIZE * size.y), z                                , type, Vector2(size.y, 1), XPOSITIF);
            ui3 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + (Block::BLOCK_SIZE * size.y), z + (Block::BLOCK_SIZE * size.z), type, Vector2(size.y, size.z), XPOSITIF);
            ui4 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y                               , z + (Block::BLOCK_SIZE * size.z), type, Vector2(1, size.z), XPOSITIF);
            renderer->addIndices(meshID, ui2, ui1, ui3);
            renderer->addIndices(meshID, ui2, ui3, ui4);
        }
    }
    if (!zNegatif) {
        size = Vector3(1);
        blocksArray[x][y][z].visited[ZNEGATIF] = true;
        for (int j = y; j + 1 < 16; j++) {
            if (blocksArray[x][j + 1][z].type != blocksArray[x][y][z].type || !blocksArray[x][j + 1][z].IsActive() || blocksArray[x][j + 1][z].visited[ZNEGATIF] || (z > 0 && (CheckIce(blocksArray[x][j + 1][z - 1], blocksArray[x][y][z]))) || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, 0, -1)) != this->chunkManager->chunkMap.end() && z == 0 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, 0, -1)]->GetBlocksArray()[x][j + 1][15], blocksArray[x][y][z]))))
                break;
            size.y += 1;
            blocksArray[x][j + 1][z].visited[ZNEGATIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = y; k < y + size.y && k < 16; k++) {
                if (blocksArray[i + 1][k][z].type != blocksArray[x][y][z].type || !blocksArray[i + 1][k][z].IsActive() || blocksArray[i + 1][k][z].visited[ZNEGATIF] || (z > 0 && (CheckIce(blocksArray[i + 1][k][z - 1], blocksArray[x][y][z]))) || (this->chunkManager->chunkMap.find(GetNormalizedPos() + Vector3(0, 0, -1)) != this->chunkManager->chunkMap.end() && z == 0 && (CheckIce(this->chunkManager->chunkMap[GetNormalizedPos() + Vector3(0, 0, -1)]->GetBlocksArray()[i + 1][k][15], blocksArray[x][y][z])))) {
                    is_row_good = false;
                    break;
                }
            }
            if (is_row_good) {
                size.x += 1;
                for (int k = y; k < y + size.y && k < 16; k++) {
                    blocksArray[i + 1][k][z].visited[ZNEGATIF] = true;
                }
            } else {
                break;
            }
        }
        if (this->blocksArray[x][y][z].type == WATER) {
            ui6 = renderer->AddVertex2(meshID, x                               , y                               , z                     , type, Vector2(1, 1), ZNEGATIF);
            ui2 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y                               , z                     , type, Vector2(size.x, 1), ZNEGATIF);
            ui1 = renderer->AddVertex2(meshID, x + (Block::BLOCK_SIZE * size.x), y + (Block::BLOCK_SIZE * size.y), z                     , type, Vector2(size.x, size.y), ZNEGATIF);
            ui5 = renderer->AddVertex2(meshID, x                               , y + (Block::BLOCK_SIZE * size.y), z                     , type, Vector2(1, size.y), ZNEGATIF);

            renderer->addIndices2(meshID, ui6, ui5, ui1);
            renderer->addIndices2(meshID, ui6, ui1, ui2);
        } else {
            ui6 = renderer->AddVertex(meshID, x                               , y                               , z                     , type, Vector2(1, 1), ZNEGATIF);
            ui2 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y                               , z                     , type, Vector2(size.x, 1), ZNEGATIF);
            ui1 = renderer->AddVertex(meshID, x + (Block::BLOCK_SIZE * size.x), y + (Block::BLOCK_SIZE * size.y), z                     , type, Vector2(size.x, size.y), ZNEGATIF);
            ui5 = renderer->AddVertex(meshID, x                               , y + (Block::BLOCK_SIZE * size.y), z                     , type, Vector2(1, size.y), ZNEGATIF);

            renderer->addIndices(meshID, ui6, ui5, ui1);
            renderer->addIndices(meshID, ui6, ui1, ui2);
        }
    }
}

bool Chunk::CheckXPositifIsVisible(int x, int y, int z) {
    Vector3 normalizedPos = GetNormalizedPos();
    bool yPositif = false;
    if (y < CHUNK_SIZE_Y - 1)
        yPositif = this->blocksArray[x][y + 1][z].IsActive();
    else if (y == CHUNK_SIZE_Y - 1 && normalizedPos.y < chunkManager->maxPos.y)
        yPositif = this->chunkManager->chunkMap[normalizedPos + Vector3(0, 1, 0)]->GetBlocksArray()[x][0][z].IsActive();
    return yPositif;
}

bool Chunk::CheckIce(Block nextBlock, Block block) {
    return (nextBlock.IsActive() && nextBlock.type != WATER) || (nextBlock.IsActive() && nextBlock.type == WATER && block.type == WATER);
}

void Chunk::CreateMesh() {
    Vector3 normalizedPos = GetNormalizedPos();
    if (!this->loaded) {
        renderer->CreateMesh(meshID);
    }
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (!this->blocksArray[x][y][z].IsActive()) {
                    continue;
                }
                // this->blocksArray[x][y][z].ClearFaces();
                bool xPositif = false;
                if (!blocksArray[x][y][z].visited[XPOSITIF]) {
                    if (x == CHUNK_SIZE_X - 1 && normalizedPos.x == chunkManager->GetMaxChunkPos().x)
                        xPositif = true;
                    else if (x < CHUNK_SIZE_X - 1)
                        xPositif = CheckIce(this->blocksArray[x + 1][y][z], this->blocksArray[x][y][z]);
                    else if (this->chunkManager->chunkMap.find(normalizedPos + Vector3(1, 0, 0)) != this->chunkManager->chunkMap.end() && x == CHUNK_SIZE_X - 1 && normalizedPos.x < chunkManager->GetMaxChunkPos().x)
                        xPositif = CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(1, 0, 0)]->GetBlocksArray()[0][y][z], this->blocksArray[x][y][z]);
                } else {
                    xPositif = true;
                }

                bool xNegatif = false;
                if (!blocksArray[x][y][z].visited[XNEGATIF]) {
                    if (x == 0 && normalizedPos.x == chunkManager->GetMinChunkPos().x)
                        xNegatif = true;
                    else if (x > 0)
                        xNegatif = CheckIce(this->blocksArray[x - 1][y][z], this->blocksArray[x][y][z]);
                    else if (this->chunkManager->chunkMap.find(normalizedPos + Vector3(-1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 0 && normalizedPos.x > chunkManager->GetMinChunkPos().x)
                        xNegatif = CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(-1, 0, 0)]->GetBlocksArray()[15][y][z], blocksArray[x][y][z]);
                } else {
                    xNegatif = true;
                }

                bool yPositif = false;
                if (!blocksArray[x][y][z].visited[YPOSITIF]) {
                    if (y == CHUNK_SIZE_Y && normalizedPos.y == chunkManager->maxPos.y)
                        yPositif = true;
                    else if (y < CHUNK_SIZE_Y - 1)
                        yPositif = CheckIce(this->blocksArray[x][y + 1][z], this->blocksArray[x][y][z]);
                    else if (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 1, 0)) != this->chunkManager->chunkMap.end() && y == CHUNK_SIZE_Y - 1 && normalizedPos.y < chunkManager->maxPos.y)
                        yPositif = CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 1, 0)]->GetBlocksArray()[x][0][z], blocksArray[x][y][z]);
                } else {
                    yPositif = true;
                }
                
                bool yNegatif = false;
                if (!blocksArray[x][y][z].visited[YNEGATIF]) {
                    if (y == 0 && normalizedPos.y == chunkManager->minPos.y)
                        yNegatif = true;
                    else if (y > 0)
                        yNegatif = CheckIce(this->blocksArray[x][y - 1][z], blocksArray[x][y][z]);
                    else if (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, -1, 0)) != this->chunkManager->chunkMap.end() && y == 0 && normalizedPos.y > chunkManager->minPos.y)
                        yNegatif = CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, -1, 0)]->GetBlocksArray()[x][15][z], this->blocksArray[x][y][z]);
                } else {
                    yNegatif = true;
                }

                bool zPositif = false;
                if (!blocksArray[x][y][z].visited[ZPOSITIF]) {
                    if (z == CHUNK_SIZE_Z - 1 && normalizedPos.z == chunkManager->GetMaxChunkPos().z)
                        zPositif = true;
                    else if (z < CHUNK_SIZE_Z - 1)
                        zPositif = CheckIce(this->blocksArray[x][y][z + 1], this->blocksArray[x][y][z]);
                    else if (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 0, 1)) != this->chunkManager->chunkMap.end() && z == CHUNK_SIZE_Z - 1 && normalizedPos.z < chunkManager->GetMaxChunkPos().z)
                        zPositif = CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, 1)]->GetBlocksArray()[x][y][0], this->blocksArray[x][y][z]);
                } else {
                    zPositif = true;
                }

                bool zNegatif = false;
                if (!blocksArray[x][y][z].visited[ZNEGATIF]) {
                    if (z == 0 && normalizedPos.z == chunkManager->GetMinChunkPos().z)
                        zNegatif = true;
                    else if (z > 0)
                        zNegatif = CheckIce(this->blocksArray[x][y][z - 1], blocksArray[x][y][z]);
                    else if (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 0, -1)) != this->chunkManager->chunkMap.end() && z == 0 && normalizedPos.z > chunkManager->GetMinChunkPos().z)
                        zNegatif = CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, -1)]->GetBlocksArray()[x][y][15], this->blocksArray[x][y][z]);
                } else {
                    zNegatif = true;
                }
                
                if (xPositif && xNegatif && yPositif && yNegatif && zPositif && zNegatif)
                    continue;
                CreateCube(x, y, z, xPositif, xNegatif, yPositif, yNegatif, zPositif, zNegatif, this->blocksArray[x][y][z].type);
            }
        }
    }
    renderer->meshes[meshID]->SetPosition(position);
    this->loaded = true;
}

void Chunk::UpdateMesh() {
    renderer->CleanMesh(meshID);

    CreateMesh();
}

void Chunk::Render() {
    renderer->Render(meshID);
}

void Chunk::Translation(Vector3 vec) {
    this->position = this->position + vec;
}

Vector3 Chunk::GetPosition() {
    return this->position;
}

Vector3 Chunk::GetNormalizedPos() {
    return this->position / 16;
}

Block ***Chunk::GetBlocksArray() {
    return blocksArray;
}