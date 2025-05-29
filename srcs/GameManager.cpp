#include "../includes/GameManager.hpp"
#include "../includes/ShadowMap.hpp"
#include <cstdlib>
#include <iostream>

GameManager::GameManager(Renderer *renderer, mapGP *tab, Player *player, ChunkManager *chunkManager, GLFWwindow *window) :
    camera(Camera(player->getGlobalPos(), Vector3(0, 1, 0))) {

    this->renderer = renderer;
    this->tab = tab;
    this->player = player;
    this->chunkManager = chunkManager;
    this->window = window;
    chunkManager->SetCamera(&camera);

    font = Font();
    renderer->InitRenderer(&camera);

    lastTime = glfwGetTime();
    deltaTime = 0;

    fpsTimer = 0;
    fps = 0;
    lastFpsInt = 60;
    lastFps = "0";

	cameraCx = (int)(camera.GetPosition().x / 16 - signeN(camera.GetPosition().x));
	cameraCz = (int)(camera.GetPosition().z / 16 - signeN(camera.GetPosition().z));
	oldx = camera.GetPosition().x;
	oldy = camera.GetPosition().y;
	oldz = camera.GetPosition().z;
    speed = 0;
	gravity = 0;
    renderer->InitSun(player);
}

GameManager::~GameManager() {

}

void GameManager::GetTimer(double &lastTime, double &deltaTime) {
    double nowTime;
    nowTime = glfwGetTime();
    deltaTime = nowTime - lastTime;
    lastTime = nowTime;
}


void GameManager::Update() {

    if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS) {
        bool debug = renderer->shadowMap->debug ? false : true;
        renderer->shadowMap->SetDebug(debug);
    }
    GetTimer(lastTime, deltaTime);
    fpsTimer += deltaTime;
    fps++;
    if (fpsTimer >= 1) {
        speed = std::sqrt((std::pow(oldz - camera.GetPosition().z, 2) + std::pow(oldy - camera.GetPosition().y, 2) + std::pow(oldx - camera.GetPosition().x, 2))) / fpsTimer;
        lastSpeed = std::to_string(speed);
        lastSpeed.resize(4);
        oldx = camera.GetPosition().x;
        oldy = camera.GetPosition().y;
        oldz = camera.GetPosition().z;

        fpsTimer = 0;
        lastFps = std::to_string(fps);
        lastFpsInt = fps;
        fps = 0;
    }
    camera.UpdateFrustum();
    font.RenderText(lastFps, 0.5, 1100, 2, Vector3(1, 0.2, 0.2));
    font.RenderText(lastSpeed, 0.5, 1000, 2, Vector3(1, 0.2, 0.2));

    camera.RegisterKeyboardInput(window, lastFpsInt);
    camera.RegisterMouseInput(window);
    chunkManager->ChunkManagerLoop();

    if (cameraCx != (int)(camera.GetPosition().x / 16 - signeN(camera.GetPosition().x))) {
        chunkManager->loadNewLine(cameraCx, camera.GetPosition().x / 16 - signeN(camera.GetPosition().x), cameraCz, player);
        cameraCx = camera.GetPosition().x / 16 - signeN(camera.GetPosition().x);
    }
    
    if (cameraCz != (int)(camera.GetPosition().z / 16 - signeN(camera.GetPosition().z))) {
        chunkManager->loadNewColumn(cameraCz, camera.GetPosition().z / 16 - signeN(camera.GetPosition().z), cameraCx, player);
        cameraCz = camera.GetPosition().z / 16 - signeN(camera.GetPosition().z);
    }

    if ((glfwGetKey(window, GLFW_KEY_F ) == GLFW_PRESS)) {
        std::cout << cameraCx << " " << cameraCz << " " << camera.GetPosition().x << " " << camera.GetPosition().z << " " << camera.GetPosition().y << '\n';
        std::cout << player->getChunkPos().x << " " << player->getChunkPos().z << '\n';
    }
    if ((glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS))
        camera.SetSpeedFps(1);
    if ((glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS))
        camera.SetSpeedFps(-1);

    if ((glfwGetKey(window, GLFW_KEY_G ) == GLFW_PRESS) && fps == 0)
        gravity = std::abs(gravity - 1);
    if (gravity == 1) {
        player->setYfromOwnChunk(camera.GetPosition());
        camera.setYfromPlayer(*player);
    } else
        player->setPos(camera.GetPosition());

}
