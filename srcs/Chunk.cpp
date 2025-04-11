#include "../includes/Chunk.hpp"
#include "../includes/Block.hpp"
#include "../includes/Vector3.hpp"
#include "../includes/Renderer.hpp"
#include "../includes/ChunkManager.hpp"

Chunk::Chunk(Renderer *renderer, ChunkManager *chunkManager) : renderer(renderer), chunkManager(chunkManager) {
    this->loaded = false;
    this->activated = true;
    this->meshed = false;
    this->meshID = 0;
    this->blocksArray = new Block**[CHUNK_SIZE_X];
    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        this->blocksArray[i] = new Block*[CHUNK_SIZE_Y];
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
            this->blocksArray[i][j] = new Block[CHUNK_SIZE_Z];
        }
    }
}

Chunk::~Chunk() {
    for (int i = 0; i < CHUNK_SIZE_X; i++) {
        for (int j = 0; j < CHUNK_SIZE_Y; j++) {
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
    /*if (position.x == 15 * 16 && position.z == 15 * 16)
        std::cout << position.y / 16 << std::endl;*/
    auto t1 = std::chrono::high_resolution_clock::now();
    Vector3 normalizedPos = GetNormalizedPos();
    renderer->CreateMesh(meshID);
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (!this->blocksArray[x][y][z].IsActive()) {
                    continue;
                }

                bool xPositif = false;
                if (normalizedPos.x == chunkManager->maxPos.x)
                    xPositif = true;
                else if (x < CHUNK_SIZE_X - 1)
                    xPositif = this->blocksArray[x + 1][y][z].IsActive();
                else if (x == CHUNK_SIZE_X - 1 && normalizedPos.x < chunkManager->maxPos.x)
                    xPositif = this->chunkManager->chunkMap[normalizedPos + Vector3(1, 0, 0)]->GetBlocksArray()[static_cast<int>(0)][y][z].IsActive();

                bool xNegatif = false;
                if (normalizedPos.x == chunkManager->minPos.x)
                    xNegatif = true;
                if (x > 0)
                    xNegatif = this->blocksArray[x - 1][y][z].IsActive();
                else if (x == 0 && normalizedPos.x > chunkManager->minPos.x)
                    xNegatif = this->chunkManager->chunkMap[normalizedPos + Vector3(-1, 0, 0)]->GetBlocksArray()[static_cast<int>(15)][y][z].IsActive();

                bool yPositif = false;
                if (y < CHUNK_SIZE_Y - 1)
                    yPositif = this->blocksArray[x][y + 1][z].IsActive();
                else if (y == CHUNK_SIZE_Y - 1 && normalizedPos.y < chunkManager->maxPos.y)
                    yPositif = this->chunkManager->chunkMap[normalizedPos + Vector3(0, 1, 0)]->GetBlocksArray()[x][static_cast<int>(0)][z].IsActive();

                bool yNegatif = false;
                if (normalizedPos.y == chunkManager->minPos.y)
                    yNegatif = true;
                else if (y > 0)
                    yNegatif = this->blocksArray[x][y - 1][z].IsActive();
                else if (y == 0 && normalizedPos.y > chunkManager->minPos.y)
                    yNegatif = this->chunkManager->chunkMap[normalizedPos + Vector3(0, -1, 0)]->GetBlocksArray()[x][static_cast<int>(15)][z].IsActive();

                bool zPositif = false;
                if (normalizedPos.z == chunkManager->maxPos.z)
                    zPositif = true;
                else if (z < CHUNK_SIZE_Z - 1)
                    zPositif = this->blocksArray[x][y][z + 1].IsActive();
                else if (z == CHUNK_SIZE_Z - 1 && normalizedPos.z < chunkManager->maxPos.z)
                    zPositif = this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, 1)]->GetBlocksArray()[x][y][static_cast<int>(0)].IsActive();

                bool zNegatif = false;
                if (normalizedPos.z == chunkManager->minPos.z)
                    zNegatif = true;
                else if (z > 0)
                    zNegatif = this->blocksArray[x][y][z - 1].IsActive();
                else if (z == 0 && normalizedPos.z > chunkManager->minPos.z)
                    zNegatif = this->chunkManager->chunkMap[normalizedPos + Vector3(0, 0, -1)]->GetBlocksArray()[x][y][static_cast<int>(15)].IsActive();
                
                if (xPositif && xNegatif && yPositif && yNegatif && zPositif && zNegatif)
                    continue;
                CreateCube(x, y, z, xPositif, xNegatif, yPositif, yNegatif, zPositif, zNegatif, this->blocksArray[x][y][z].type);
            }
        }
    }
    //renderer->FinishMesh(meshID);
    renderer->meshes[meshID]->SetPosition(position);
    this->loaded = true;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    //std::cout << "double = " << ms_double.count() << std::endl;
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