#include "../includes/Chunk.hpp"
#include "../includes/Block.hpp"
#include "../includes/Vector3.hpp"
#include "../includes/Renderer.hpp"
#include "../includes/ChunkManager.hpp"

Chunk::Chunk(Renderer *renderer, ChunkManager *chunkManager, unsigned char ***test) : renderer(renderer), chunkManager(chunkManager) {
    this->loaded = false;
    this->activated = true;
    this->meshed = false;
    this->update = false;
    this->meshID = UINTMAX;
    Block *block;
    unsigned char texture;
    this->blocksArray = new Block**[CHUNK_SIZE_X];
    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        this->blocksArray[i] = new Block*[CHUNK_SIZE_Y];
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            this->blocksArray[i][j] = new Block[CHUNK_SIZE_Z];
            for (int k = 0; k < CHUNK_SIZE_Z; k++) {
                texture = test[i][k][j];
                block  = &this->blocksArray[i][j][k];
                block->SetActive(texture);
                block->type = static_cast<BlockType>(texture);
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

void Chunk::CleanChunk(bool withPos) {
    renderer->CleanMesh(meshID);
    if (withPos)
        position = Vector3(0, 0, 0);
}

void Chunk::NewChunk(unsigned char ***test) {
    unsigned char test0;
    Block *block;
    this->loaded = false;
    this->activated = true;
    this->meshed = false;
    this->update = false;
    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            for (int k = 0; k < CHUNK_SIZE_Z; k++) {
                block = &this->blocksArray[i][j][k];
                test0 = test[i][k][j];
                block->SetActive(test0);
                block->type = static_cast<BlockType>(test0);
                block->ClearFaces();
            }
        }
    }

}

void Chunk::CreateCube(Vector3 &normalizedPos, int &x, int &y, int &z, bool &xPositif, bool &xNegatif, bool &yPositif, bool &yNegatif, bool &zPositif, bool &zNegatif, int type) {
    unsigned int ui1 = 0;
    unsigned int ui2 = 0;
    unsigned int ui3 = 0;
    unsigned int ui4 = 0;
    unsigned int ui5 = 0;
    unsigned int ui6 = 0;
    unsigned int ui7 = 0;
    unsigned int ui8 = 0;
    Vector3 size(1);
    Block *block = &blocksArray[x][y][z];
    if (!yPositif) {
        size = Vector3(1);
        block->visited[YPOSITIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != block->type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[YPOSITIF] || (y + 1 < 16 && CheckIce(this->blocksArray[x][y + 1][j + 1], block)) || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 1, 0)) != this->chunkManager->chunkMap.end() && y == 15 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 1, 0)]->GetBlocksArray()[x][0][j + 1], block))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[YPOSITIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[i + 1][y][k].type != block->type || !blocksArray[i + 1][y][k].IsActive() || blocksArray[i + 1][y][k].visited[YPOSITIF] || (y + 1 < 16 && CheckIce(blocksArray[i + 1][y + 1][k], block))  || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 1, 0)) != this->chunkManager->chunkMap.end() && y == 15 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 1, 0)]->GetBlocksArray()[i + 1][0][k], block)))) {
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
        if (block->type == WATER) {
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
        block->visited[ZPOSITIF] = true;
        for (int j = y; j + 1 < 16; j++) {
            if (blocksArray[x][j + 1][z].type != block->type || !blocksArray[x][j + 1][z].IsActive() || blocksArray[x][j + 1][z].visited[ZPOSITIF] || (z + 1 < 16 && (CheckIce(blocksArray[x][j + 1][z + 1], block)))  || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 0, 1)) != this->chunkManager->chunkMap.end() && z == 15 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, 1)]->GetBlocksArray()[x][j + 1][0], block))))
                break;
            size.y += 1;
            blocksArray[x][j + 1][z].visited[ZPOSITIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = y; k < y + size.y && k < 16; k++) {
                if (blocksArray[i + 1][k][z].type != block->type || !blocksArray[i + 1][k][z].IsActive() || blocksArray[i + 1][k][z].visited[ZPOSITIF] || (z + 1 < 16 && (CheckIce(blocksArray[i + 1][k][z + 1], block)))  || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 0, 1)) != this->chunkManager->chunkMap.end() && z == 15 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, 1)]->GetBlocksArray()[i + 1][k][0], block)))) {
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
        if (block->type == WATER) {
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
        block->visited[XNEGATIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != block->type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[XNEGATIF] || (x > 0 && (CheckIce(blocksArray[x - 1][y][j + 1], block)))  || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(-1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 0 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(-1, 0, 0)]->GetBlocksArray()[15][y][j + 1], block))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[XNEGATIF] = true;
        }
        for (int i = y; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[x][i + 1][k].type != block->type || !blocksArray[x][i + 1][k].IsActive() || blocksArray[x][i + 1][k].visited[XNEGATIF] || (x > 0 && (CheckIce(blocksArray[x - 1][i + 1][k], block)))  || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(-1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 0 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(-1, 0, 0)]->GetBlocksArray()[15][i + 1][k], block)))) {
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
        if (block->type == WATER) {
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
        block->visited[YNEGATIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != block->type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[YNEGATIF] || (y > 0 && (CheckIce(blocksArray[x][y - 1][j + 1], block))) || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, -1, 0)) != this->chunkManager->chunkMap.end() && y == 0 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, -1, 0)]->GetBlocksArray()[x][15][j + 1], block))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[YNEGATIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[i + 1][y][k].type != block->type || !blocksArray[i + 1][y][k].IsActive() || blocksArray[i + 1][y][k].visited[YNEGATIF] || (y > 0 && (CheckIce(blocksArray[i + 1][y - 1][k], block))) || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, -1, 0)) != this->chunkManager->chunkMap.end() && y == 0 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, -1, 0)]->GetBlocksArray()[i + 1][15][k], block)))) {
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
        if (block->type == WATER) {
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
        block->visited[XPOSITIF] = true;
        for (int j = z; j + 1 < 16; j++) {
            if (blocksArray[x][y][j + 1].type != block->type || !blocksArray[x][y][j + 1].IsActive() || blocksArray[x][y][j + 1].visited[XPOSITIF] || (x + 1 < 16 && (CheckIce(blocksArray[x + 1][y][j + 1], block))) || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 15 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(1, 0, 0)]->GetBlocksArray()[0][y][j + 1], block))))
                break;
            size.z += 1;
            blocksArray[x][y][j + 1].visited[XPOSITIF] = true;
        }
        for (int i = y; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = z; k < z + size.z && k < 16; k++) {
                if (blocksArray[x][i + 1][k].type != block->type || !blocksArray[x][i + 1][k].IsActive() || blocksArray[x][i + 1][k].visited[XPOSITIF] || (x + 1 < 16 && (CheckIce(blocksArray[x + 1][i + 1][k], block))) || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(1, 0, 0)) != this->chunkManager->chunkMap.end() && x == 15 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(1, 0, 0)]->GetBlocksArray()[0][i + 1][k], block)))) {
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
        if (block->type == WATER) {
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
        block->visited[ZNEGATIF] = true;
        for (int j = y; j + 1 < 16; j++) {
            if (blocksArray[x][j + 1][z].type != block->type || !blocksArray[x][j + 1][z].IsActive() || blocksArray[x][j + 1][z].visited[ZNEGATIF] || (z > 0 && (CheckIce(blocksArray[x][j + 1][z - 1], block))) || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 0, -1)) != this->chunkManager->chunkMap.end() && z == 0 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, -1)]->GetBlocksArray()[x][j + 1][15], block))))
                break;
            size.y += 1;
            blocksArray[x][j + 1][z].visited[ZNEGATIF] = true;
        }
        for (int i = x; i + 1 < 16; i++) {
            bool is_row_good = true;
            for (int k = y; k < y + size.y && k < 16; k++) {
                if (blocksArray[i + 1][k][z].type != block->type || !blocksArray[i + 1][k][z].IsActive() || blocksArray[i + 1][k][z].visited[ZNEGATIF] || (z > 0 && (CheckIce(blocksArray[i + 1][k][z - 1], block))) || (this->chunkManager->chunkMap.find(normalizedPos + Vector3(0, 0, -1)) != this->chunkManager->chunkMap.end() && z == 0 && (CheckIce(this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, -1)]->GetBlocksArray()[i + 1][k][15], block)))) {
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
        if (block->type == WATER) {
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

bool Chunk::CheckIce(Block &nextBlock, Block *block) {
    return (nextBlock.IsActive() && nextBlock.type != WATER) || (nextBlock.IsActive() && nextBlock.type == WATER && block->type == WATER);
}

void Chunk::CreateMesh() {
    Vector3 normalizedPos = GetNormalizedPos();
    if (!this->loaded && this->meshID == UINTMAX) {
        renderer->CreateMesh(meshID);
    }
    Block *block;
    Vector3 maxChunkPos = chunkManager->GetMaxChunkPos();
    Vector3 minChunkPos = chunkManager->GetMinChunkPos();

    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                block = &this->blocksArray[x][y][z];
                if (this->update)
                    block->ClearFaces();
                if (!block->IsActive()) {
                    continue;
                }
                bool xPositif = false;
                if (!block->visited[XPOSITIF]) {
                    if (x < CHUNK_SIZE_X - 1)
                        xPositif = CheckIce(this->blocksArray[x + 1][y][z], block);
                    else if (x == CHUNK_SIZE_X - 1 && normalizedPos.x == maxChunkPos.x)
                        xPositif = true;
                    else if (this->chunkManager->chunkMap.find(normalizedPos + directionVecs[0]) != this->chunkManager->chunkMap.end() && x == CHUNK_SIZE_X - 1 && normalizedPos.x < maxChunkPos.x)
                        xPositif = CheckIce(this->chunkManager->chunkMap[normalizedPos + directionVecs[0]]->GetBlocksArray()[0][y][z], block);
                } else {
                    xPositif = true;
                }

                bool xNegatif = false;
                if (!block->visited[XNEGATIF]) {
                    if (x > 0)
                        xNegatif = CheckIce(this->blocksArray[x - 1][y][z], block);
                    else if (x == 0 && normalizedPos.x == minChunkPos.x)
                        xNegatif = true;
                    else if (this->chunkManager->chunkMap.find(normalizedPos + directionVecs[1]) != this->chunkManager->chunkMap.end() && x == 0 && normalizedPos.x > minChunkPos.x)
                        xNegatif = CheckIce(this->chunkManager->chunkMap[normalizedPos + directionVecs[1]]->GetBlocksArray()[15][y][z], block);
                } else {
                    xNegatif = true;
                }

                bool yPositif = false;
                if (!block->visited[YPOSITIF]) {
                    if (y < CHUNK_SIZE_Y - 1)
                        yPositif = CheckIce(this->blocksArray[x][y + 1][z], block);
                    else if (y == CHUNK_SIZE_Y && normalizedPos.y == chunkManager->maxPos.y)
                        yPositif = true;
                    else if (this->chunkManager->chunkMap.find(normalizedPos + directionVecs[2]) != this->chunkManager->chunkMap.end() && y == CHUNK_SIZE_Y - 1 && normalizedPos.y < chunkManager->maxPos.y)
                        yPositif = CheckIce(this->chunkManager->chunkMap[normalizedPos + directionVecs[2]]->GetBlocksArray()[x][0][z], block);
                } else {
                    yPositif = true;
                }
                
                bool yNegatif = false;
                if (!block->visited[YNEGATIF]) {
                    if (y > 0)
                        yNegatif = CheckIce(this->blocksArray[x][y - 1][z], block);
                    else if (y == 0 && normalizedPos.y == chunkManager->minPos.y)
                        yNegatif = true;
                    else if (this->chunkManager->chunkMap.find(normalizedPos + directionVecs[3]) != this->chunkManager->chunkMap.end() && y == 0 && normalizedPos.y > chunkManager->minPos.y)
                        yNegatif = CheckIce(this->chunkManager->chunkMap[normalizedPos + directionVecs[3]]->GetBlocksArray()[x][15][z], block);
                } else {
                    yNegatif = true;
                }

                bool zPositif = false;
                if (!block->visited[ZPOSITIF]) {
                    if (z < CHUNK_SIZE_Z - 1)
                        zPositif = CheckIce(this->blocksArray[x][y][z + 1], block);
                    else if (z == CHUNK_SIZE_Z - 1 && normalizedPos.z == maxChunkPos.z)
                        zPositif = true;
                    else if (this->chunkManager->chunkMap.find(normalizedPos + directionVecs[4]) != this->chunkManager->chunkMap.end() && z == CHUNK_SIZE_Z - 1 && normalizedPos.z < maxChunkPos.z)
                        zPositif = CheckIce(this->chunkManager->chunkMap[normalizedPos + directionVecs[4]]->GetBlocksArray()[x][y][0], block);
                } else {
                    zPositif = true;
                }

                bool zNegatif = false;
                if (!block->visited[ZNEGATIF]) {
                    if (z > 0)
                        zNegatif = CheckIce(this->blocksArray[x][y][z - 1], block);
                    else if (z == 0 && normalizedPos.z == minChunkPos.z)
                        zNegatif = true;
                    else if (this->chunkManager->chunkMap.find(normalizedPos + directionVecs[5]) != this->chunkManager->chunkMap.end() && z == 0 && normalizedPos.z > minChunkPos.z)
                        zNegatif = CheckIce(this->chunkManager->chunkMap[normalizedPos + directionVecs[5]]->GetBlocksArray()[x][y][15], block);
                } else {
                    zNegatif = true;
                }
                
                if (xPositif && xNegatif && yPositif && yNegatif && zPositif && zNegatif)
                    continue;
                CreateCube(normalizedPos,  x, y, z, xPositif, xNegatif, yPositif, yNegatif, zPositif, zNegatif, block->type);
                renderer->meshes[meshID]->SetPosition(position);
            this->loaded = true;
            }
        }
    }
}

bool Chunk::CubeRayCast(Camera *camera) {
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                Block *blockTmp = &this->GetBlocksArray()[x][y][z];
                if (!blockTmp->IsActive())
                    continue;
                AABB blockAABB;
                blockAABB.center[0] = this->GetPosition().x + x;
                blockAABB.extents[0] = this->GetPosition().x + x + 1;
                blockAABB.center[1] = this->GetPosition().y + y;
                blockAABB.extents[1] = this->GetPosition().y + y + 1;
                blockAABB.center[2] = this->GetPosition().z + z;
                blockAABB.extents[2] = this->GetPosition().z + z + 1;
                if (camera->AABBInterstect(blockAABB)) {
                    blockTmp->type = BlockType::DEFAULT;
                    blockTmp->SetActive(false);
                    this->update = true;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Chunk::CubeRayCast(Camera *camera, Vector3i &cubePos) {
    if (cubePos.x < 0)
        cubePos.x = 16 + cubePos.x;
    if (cubePos.y < 0)
        cubePos.y = 16 + cubePos.y;
    if (cubePos.z < 0)
        cubePos.z = 16 + cubePos.z;
    Block *blockTmp = &this->GetBlocksArray()[cubePos.x][cubePos.y][cubePos.z];
    if (!blockTmp->IsActive())
        return false;
    AABB blockAABB;
    blockAABB.center[0] = this->GetPosition().x + cubePos.x;
    blockAABB.extents[0] = this->GetPosition().x + cubePos.x + 1;
    blockAABB.center[1] = this->GetPosition().y + cubePos.y;
    blockAABB.extents[1] = this->GetPosition().y + cubePos.y + 1;
    blockAABB.center[2] = this->GetPosition().z + cubePos.z;
    blockAABB.extents[2] = this->GetPosition().z + cubePos.z + 1;
    if (camera->AABBInterstect(blockAABB)) {
        Vector3 str = Vector3(cubePos.x, cubePos.y, cubePos.z);
        blockTmp->type = BlockType::DEFAULT;
        blockTmp->SetActive(false);
        this->update = true;
        return true;
    }
    return false;
}

void Chunk::UpdateMesh() {
    renderer->CleanMesh(meshID);

    CreateMesh();
}

void Chunk::Translation(Vector3 vec) {
    this->position = vec;
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

void Chunk::PrintBlocksArray() {
    std::cout << "Print Block array " << std::endl;
     for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            std::string tmp = "line = ";
            for (int k = 0; k < CHUNK_SIZE_Z; k++) {
                Block block = this->blocksArray[i][j][k];
                tmp.append(std::to_string(block.type) + " ");
            }
            std::cout << tmp << std::endl;
        }
    }
}
