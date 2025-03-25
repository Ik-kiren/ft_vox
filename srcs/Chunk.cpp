#include "../includes/Chunk.hpp"
#include "../includes/Block.hpp"
#include "../includes/Vector3.hpp"

Chunk::Chunk(Renderer &renderer) : renderer(renderer) {
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

void Chunk::CreateCube(int x, int y, int z, bool xPositif, bool xNegatif, bool yPositif, bool yNegatif, bool zPositif, bool zNegatif) {
    Vector3 v1 = {x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 v2 = {x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 v3 = {x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 v4 = {x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 v5 = {x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 v6 = {x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE};
    Vector3 v7 = {x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 v8 = {x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE};
    Vector3 n1;
    unsigned int ui1 = 0;
    unsigned int ui2 = 0;
    unsigned int ui3 = 0;
    unsigned int ui4 = 0;
    unsigned int ui5 = 0;
    unsigned int ui6 = 0;
    unsigned int ui7 = 0;
    unsigned int ui8 = 0;
    /*ui1 = renderer.AddVertex(meshID, v1);
    ui2 = renderer.AddVertex(meshID, v2);
    ui3 = renderer.AddVertex(meshID, v3);
    ui4 = renderer.AddVertex(meshID, v4);
    ui5 = renderer.AddVertex(meshID, v5);
    ui6 = renderer.AddVertex(meshID, v6);
    ui7 = renderer.AddVertex(meshID, v7);
    ui8 = renderer.AddVertex(meshID, v8);*/

    ui1 = renderer.AddVertex(meshID, v1);
    ui5 = renderer.AddVertex(meshID, v5);
    ui7 = renderer.AddVertex(meshID, v7);
    ui3 = renderer.AddVertex(meshID, v3);
    if (!yPositif) {
        renderer.addIndices(meshID, ui1, ui5, ui7);
        renderer.addIndices(meshID, ui1, ui7, ui3);
    }
    ui4 = renderer.AddVertex(meshID, v4);
    ui3 = renderer.AddVertex(meshID, v3);
    ui7 = renderer.AddVertex(meshID, v7);
    ui8 = renderer.AddVertex(meshID, v8);
    if (!zPositif) {
        renderer.addIndices(meshID, ui4, ui3, ui7);
        renderer.addIndices(meshID, ui4, ui7, ui8);
    }
    ui8 = renderer.AddVertex(meshID, v8);
    ui7 = renderer.AddVertex(meshID, v7);
    ui5 = renderer.AddVertex(meshID, v5);
    ui6 = renderer.AddVertex(meshID, v6);
    if (!xNegatif) {
        renderer.addIndices(meshID, ui8, ui7, ui5);
        renderer.addIndices(meshID, ui8, ui5, ui6);
    }
    ui6 = renderer.AddVertex(meshID, v6);
    ui2 = renderer.AddVertex(meshID, v2);
    ui4 = renderer.AddVertex(meshID, v4);
    ui8 = renderer.AddVertex(meshID, v8);
    if (!yNegatif) {
        renderer.addIndices(meshID, ui6, ui2, ui4);
        renderer.addIndices(meshID, ui6, ui4, ui8);
    }
    ui2 = renderer.AddVertex(meshID, v2);
    ui1 = renderer.AddVertex(meshID, v1);
    ui3 = renderer.AddVertex(meshID, v3);
    ui4 = renderer.AddVertex(meshID, v4);
    if (!xPositif) {
        renderer.addIndices(meshID, ui2, ui1, ui3);
        renderer.addIndices(meshID, ui2, ui3, ui4);
    }
    ui6 = renderer.AddVertex(meshID, v6);
    ui5 = renderer.AddVertex(meshID, v5);
    ui1 = renderer.AddVertex(meshID, v1);
    ui2 = renderer.AddVertex(meshID, v2);
    if (!zNegatif) {
        renderer.addIndices(meshID, ui6, ui5, ui1);
        renderer.addIndices(meshID, ui6, ui1, ui2);
    }
}

void Chunk::CreateMesh() {
    renderer.CreateMesh(meshID);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (!this->blocksArray[x][y][z].IsActive()) {
                    continue;
                }
                bool xPositif = false;
                if (x < CHUNK_SIZE - 1)
                    xPositif = this->blocksArray[x + 1][y][z].IsActive();
                bool xNegatif = false;
                if (x > 0)
                xNegatif = this->blocksArray[x - 1][y][z].IsActive();

                bool yPositif = false;
                if (y < CHUNK_SIZE - 1)
                    yPositif = this->blocksArray[x][y + 1][z].IsActive();
                bool yNegatif = false;
                if (y > 0)
                    yNegatif = this->blocksArray[x][y - 1][z].IsActive();

                bool zPositif = false;
                if (z < CHUNK_SIZE - 1)
                    zPositif = this->blocksArray[x][y][z + 1].IsActive();
                bool zNegatif = false;
                if (z > 0)
                    zNegatif = this->blocksArray[x][y][z - 1].IsActive();              
                if (xPositif && xNegatif && yPositif && yNegatif && zPositif && zNegatif)
                    continue;
                CreateCube(x, y, z, xPositif, xNegatif, yPositif, yNegatif, zPositif, zNegatif);
            }
        }
    }
    renderer.FinishMesh(meshID);
}

void Chunk::Render() {
    renderer.Render(meshID);
}

void Chunk::Translation(Vector3 vec) {
    this->position = this->position + vec;
    renderer.meshes[meshID].SetPosition(position);
}

Block ***Chunk::GetBlocksArray() {
    return blocksArray;
}