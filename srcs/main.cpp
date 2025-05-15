#include <cmath>
#include <iostream>
#include <string>
#include <stack>
#include "../includes/Scop.hpp"
#include "../includes/Camera.hpp"
#include "../includes/Chunk.hpp"
#include "../includes/ChunkManager.hpp"
#include "../includes/Renderer.hpp"
#include "../includes/mapGP.hpp"
#include <chrono>

#include <sys/time.h>

int seed = 0;


void viewport_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

GLFWwindow *InitGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(0);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(1920, 1200, "window", NULL, NULL);
    if (!window) {
        std::cerr << "failed to create window" << std::endl;
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "failed to initialize GLEW" << std::endl;
        glfwTerminate();
        exit(0);
    }
    glfwSetFramebufferSizeCallback(window, viewport_size_callback);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glfwSwapInterval(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    return window;
}

void GetTimer(double &lastTime, double &deltaTime) {
    double nowTime;
    nowTime = glfwGetTime();
    deltaTime = nowTime - lastTime;
    lastTime = nowTime;
}

int main(void) {
    srand(time(NULL));
	// seed = std::rand();
	seed = 255;
    Renderer renderer;

	struct timeval tp0;
	gettimeofday(&tp0, NULL);
    // mapGP tab(65, 16);
    mapGP tab(32, 32);
	struct timeval tp1;
	gettimeofday(&tp1, NULL);
	std::cout << tp1.tv_sec - tp0.tv_sec << " sec " << tp1.tv_usec - tp0.tv_usec << " ms\n";

	Player	player(0, 128, 0);

	chunk *pos = tab.chunkToRet(player.getPos().x, player.getPos().y);
	player.setChunk(pos);

    ChunkManager test(&renderer, &tab, &player);
	test.Init();

    GLFWwindow *window;
    window = InitGLFW();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader cubeShader = Shader("./shaders/VertexShader.shader", "./shaders/FragmentShader2.shader");
    Shader fontShader = Shader("./shaders/fontVS.shader", "./shaders/fontFS.shader");

    //Mesh cubeMesh = Mesh("./objects/DirtCube.obj");  

    Camera camera = Camera(Vector3(player.getPos().x + 0.5, player.getPos().y, player.getPos().z + 0.5), Vector3(0, 1, 0));
    test.SetCamera(&camera);

    //Object cubeObj = Object(cubeShader, &cubeMesh, Vector4(1, 1, 1, 1));

    Font font = Font();
    renderer.InitRenderer(&cubeShader, &camera);

    double lastTime = 0;
    double deltaTime = 0;

    double actionTimer = 0;
    double xpos = 0;
    double ypos = 0;
    int windowWidth = 0;
    int windowHeigth = 0;
    float fpsTimer = 0;
    int fps = 0;
    int	lastFpsInt = 60;
    std::string lastFps = "0";

	int	cameraCx = camera.GetPosition().x / 16;
	int	cameraCz = camera.GetPosition().z / 16;
	float	oldx = camera.GetPosition().x;
	float	oldy = camera.GetPosition().y;
	float	oldz = camera.GetPosition().z;
	float	speed = 0;
	std::string	lastSpeed;
	int		gravity = 0;

    while ((glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0)) {
            glClearColor(0.0f, 0.45f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            glfwGetCursorPos(window, &xpos, &ypos);
            ypos = abs(ypos - windowHeigth);
            glfwGetWindowSize(window, &windowWidth, &windowHeigth);
            GetTimer(lastTime, deltaTime);
            actionTimer += deltaTime;
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
            font.RenderText(fontShader, lastFps, 0.5, 1100, 2, Vector3(1, 0.2, 0.2));
			font.RenderText(fontShader, lastSpeed, 0.5, 1000, 2, Vector3(1, 0.2, 0.2));
            
            //cubeObj.drawMeshInstance(window, camera, objects, compute);
			
            //Vector3 camPos = camera.GetPosition() / 16;
            //std::cout << camPos << std::endl;
            // camera.RegisterKeyboardInput(window);
            // camera.RegisterKeyboardInput(window, deltaTime);
            camera.RegisterKeyboardInput(window, lastFpsInt);
            camera.RegisterMouseInput(window);
            test.ChunkManagerLoop();

			if (cameraCx != (int)(camera.GetPosition().x / 16 - signeN(camera.GetPosition().x))) {
				test.loadNewLine(cameraCx, camera.GetPosition().x / 16 - signeN(camera.GetPosition().x), cameraCz, &player);
				cameraCx = camera.GetPosition().x / 16 - signeN(camera.GetPosition().x);
			}
			
			if (cameraCz != (int)(camera.GetPosition().z / 16 - signeN(camera.GetPosition().z))) {
				test.loadNewColumn(cameraCz, camera.GetPosition().z / 16 - signeN(camera.GetPosition().z), cameraCx, &player);
				cameraCz = camera.GetPosition().z / 16 - signeN(camera.GetPosition().z);
			}

			if ((glfwGetKey(window, GLFW_KEY_F ) == GLFW_PRESS)) {
				std::cout << cameraCx << " " << cameraCz << " " << camera.GetPosition().x << " " << camera.GetPosition().z << " " << camera.GetPosition().y << '\n';
				std::cout << player.getPos().x << " " << player.getPos().z << '\n';
			}
			if ((glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS))
				camera.SetSpeedFps(1);
			if ((glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS))
				camera.SetSpeedFps(-1);
			// if ((glfwGetKey(window, GLFW_KEY_C ) == GLFW_PRESS))
			// 	test.deleteCube(camera);

			if ((glfwGetKey(window, GLFW_KEY_G ) == GLFW_PRESS) && fps == 0)
				gravity = std::abs(gravity - 1);
			if (gravity == 1) {
				player.setYfromOwnChunk(camera.GetPosition());
				camera.setYfromPlayer(player);
			} else
				player.setPos(camera.GetPosition());

			glfwSwapBuffers(window);
			glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
