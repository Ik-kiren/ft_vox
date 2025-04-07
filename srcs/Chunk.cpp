#include "../includes/Chunk.hpp"
#include "../includes/Block.hpp"
#include "../includes/Vector3.hpp"
#include "../includes/Renderer.hpp"

Chunk::Chunk(Renderer *renderer) : renderer(renderer) {
    this->loaded = false;
    this->activated = true;
    this->meshed = false;
    this->meshID = 0;
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

void Chunk::CreateCube(int &x, int &y, int &z, bool &xPositif, bool &xNegatif, bool &yPositif, bool &yNegatif, bool &zPositif, bool &zNegatif, float type) {
    unsigned int ui1 = 0;
    unsigned int ui2 = 0;
    unsigned int ui3 = 0;
    unsigned int ui4 = 0;
    unsigned int ui5 = 0;
    unsigned int ui6 = 0;
    unsigned int ui7 = 0;
    unsigned int ui8 = 0;
    if (!yPositif) {
        ui1 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui5 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui7 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui3 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        renderer->addIndices(meshID, ui1, ui5, ui7);
        renderer->addIndices(meshID, ui1, ui7, ui3);
    }
    if (!zPositif) {
        ui4 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui3 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui7 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui8 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        renderer->addIndices(meshID, ui4, ui3, ui7);
        renderer->addIndices(meshID, ui4, ui7, ui8);
    }
    if (!xNegatif) {
        ui8 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui7 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui5 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui6 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        renderer->addIndices(meshID, ui8, ui7, ui5);
        renderer->addIndices(meshID, ui8, ui5, ui6);
    }
    if (!yNegatif) {
        ui6 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui2 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui4 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui8 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        renderer->addIndices(meshID, ui6, ui2, ui4);
        renderer->addIndices(meshID, ui6, ui4, ui8);
    }
    if (!xPositif) {
        ui2 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui1 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui3 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        ui4 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z + Block::BLOCK_SIZE, type);
        renderer->addIndices(meshID, ui2, ui1, ui3);
        renderer->addIndices(meshID, ui2, ui3, ui4);
    }
    if (!zNegatif) {
        ui6 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui5 = renderer->AddVertex(meshID, x - Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui1 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y + Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        ui2 = renderer->AddVertex(meshID, x + Block::BLOCK_SIZE, y - Block::BLOCK_SIZE, z - Block::BLOCK_SIZE, type);
        renderer->addIndices(meshID, ui6, ui5, ui1);
        renderer->addIndices(meshID, ui6, ui1, ui2);
    }
}

void Chunk::CreateMesh() {
    renderer->CreateMesh(meshID);
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
                CreateCube(x, y, z, xPositif, xNegatif, yPositif, yNegatif, zPositif, zNegatif, this->blocksArray[x][y][z].type);
            }
        }
    }
    //renderer->FinishMesh(meshID);
    renderer->meshes[meshID]->SetPosition(position);
    this->loaded = true;
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

Block ***Chunk::GetBlocksArray() {
    return blocksArray;
}