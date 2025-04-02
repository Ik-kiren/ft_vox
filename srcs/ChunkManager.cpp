#include "../includes/ChunkManager.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/Renderer.hpp"

ChunkManager::ChunkManager(Renderer *renderer, Camera *camera): renderer(renderer), camera(camera) {
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            for (int k = 0; k < 5; k++) {
                Chunk *newChunk = new Chunk(renderer);
                newChunk->Translation(Vector3(i * 16, j * 16, k * 16 - 32));
                loadList.push_back(newChunk);
            }
        }
    }
}

ChunkManager::~ChunkManager() {
    for (size_t i = 0; i < loadList.size(); i++)
    {
        delete loadList[i];
    }
    
}

void ChunkManager::LoadChunk() {
    (void)renderer;
    Chunk *nextToLoad = NULL;
    for (std::vector<Chunk *>::iterator it = loadList.begin(); it != loadList.end(); it++) {
        if ((*it)->loaded == false) {
            nextToLoad = *it;
            break;
        }
    }
    if (nextToLoad)
        nextToLoad->CreateMesh();
}



void ChunkManager::ChunkVisibility() {

    visibilityList.clear();
    for (size_t i = 0; i < 1; i++) {
        BSphere bsphere;
        Vector3 vec = oneToThree(i, 16, 16, 16);
        vec = vec * Vector3(16, 16, 16) + Vector3(8, 8, 8 - 32);
        bsphere.center[0] = vec.x;
        bsphere.center[1] = vec.y;
        bsphere.center[2] = vec.z;
        bsphere.radius = 8;
        Vector3 front = camera->GetRight();
        //std::cout << "front " << front << std::endl;
        if (camera->InsideFrustum(bsphere)) {
            visibilityList.push_back(loadList[i]);
        }
        std::cout << visibilityList.size() << std::endl;
    }
    renderer->Render(visibilityList);
    lastCamPos = camera->GetPosition();
}