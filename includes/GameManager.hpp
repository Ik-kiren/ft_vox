#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "./Renderer.hpp"
#include "./mapGP.hpp"
#include "./Player.hpp"
#include "./ChunkManager.hpp"
#include "./Constants.hpp"


class GameManager
{
private:
    Renderer *renderer;
    mapGP *tab;
    Player *player;
    ChunkManager *chunkManager;


    //void InitGLFW();
    void GetTimer(double &lastTime, double &deltaTime);
public:
    GLFWwindow *window;
    static float windowWidth;
    static float windowHeight;

    Camera camera;
    Font font;

    double deltaTime;
    double fpsTimer;
    double lastTime;
    int fps;
    int lastFpsInt;
    std::string lastFps;

    int	cameraCx;
	int	cameraCz;
	float	oldx;
	float	oldy;
	float	oldz;
	float	speed;
	std::string	lastSpeed;
	int		gravity;

    GameManager(Renderer *renderer, mapGP *tab, Player *player, ChunkManager *chunkManager, GLFWwindow *window);
    ~GameManager();

    void Update();
};
