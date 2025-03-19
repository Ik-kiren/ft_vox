#include "../includes/Chunk.hpp"
#include "../includes/Block.hpp"
#include "../includes/Vector3.hpp"

Chunk::Chunk() {
    this->blocksArray = new Block**[CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; i++) {
        this->blocksArray[i] = new Block*[CHUNK_SIZE];
        for (int j = 0; j < CHUNK_SIZE; j++) {
            this->blocksArray[i][j] = new Block[CHUNK_SIZE];
        }
    }
}

Chunk::~Chunk() {
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            delete[] this->blocksArray[i][j]; 
        }
        delete[] this->blocksArray[i];
    }
    delete[] this->blocksArray;
}

void Chunk::CreateCube(int x, int y, int z) {
    Vector3 v1 = {x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 v2 = {x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 v3 = {x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 v4 = {x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 v5 = {x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 v6 = {x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 v7 = {x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 v8 = {x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 n1;
    unsigned int ui1;
    unsigned int ui2;
    unsigned int ui3;
    unsigned int ui4;
    unsigned int ui5;
    unsigned int ui6;
    unsigned int ui7;
    unsigned int ui8;
    ui1 = renderer.AddVertex(meshID, v1);
    ui2 = renderer.AddVertex(meshID, v2);
    ui3 = renderer.AddVertex(meshID, v3);
    ui4 = renderer.AddVertex(meshID, v4);
    ui5 = renderer.AddVertex(meshID, v5);
    ui6 = renderer.AddVertex(meshID, v6);
    ui7 = renderer.AddVertex(meshID, v7);
    ui8 = renderer.AddVertex(meshID, v8);
    renderer.addIndices(meshID, ui1, ui2, ui3);
    renderer.addIndices(meshID, ui1, ui3, ui4);
    renderer.addIndices(meshID, ui5, ui6, ui7);
    renderer.addIndices(meshID, ui5, ui7, ui8);
    renderer.addIndices(meshID, ui2, ui5, ui8);
    renderer.addIndices(meshID, ui2, ui8, ui3);
    renderer.addIndices(meshID, ui6, ui1, ui4);
    renderer.addIndices(meshID, ui6, ui4, ui7);
    renderer.addIndices(meshID, ui4, ui3, ui8);
    renderer.addIndices(meshID, ui4, ui8, ui7);
    renderer.addIndices(meshID, ui6, ui5, ui2);
    renderer.addIndices(meshID, ui6, ui2, ui1);
}

void Chunk::CreateMesh() {
    renderer.CreateMesh(meshID);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (!this->blocksArray[x][y][z].IsActive()) {
                    continue;
                }
                CreateCube(x, y, z);
            }
        }
    }
    renderer.FinishMesh(meshID);
}