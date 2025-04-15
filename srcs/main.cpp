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
	seed = 100;
    Renderer renderer;

    mapGP tab(48, 16);
	coord2d start = gene2D(256, 256);
    chunk ***monoC = tab.chunkToRet(start.x, start.y);
    ChunkManager test(&renderer, monoC);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (i == 0 && j == 0)
				continue ;
			chunk ***monoCx = tab.chunkToRet(start.x + i, start.y + j);
			test.loadNewChunk(monoCx, i, j);
		}
	}
	test.Init();

    GLFWwindow *window;
    window = InitGLFW();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader cubeShader = Shader("./shaders/VertexShader.shader", "./shaders/FragmentShader2.shader");
    Shader fontShader = Shader("./shaders/fontVS.shader", "./shaders/fontFS.shader");
    ComputeShader compute = ComputeShader("./shaders/compute.shader");

    //Mesh cubeMesh = Mesh("./objects/DirtCube.obj");  

    Camera camera = Camera(Vector3(0, 128, 0), Vector3(0, 1, 0));

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
    std::string lastFps = "0";
	int ha = 0;

    while ((glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0)) {
            glClearColor(0.0f, 0.45f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwGetCursorPos(window, &xpos, &ypos);
            ypos = abs(ypos - windowHeigth);
            glfwGetWindowSize(window, &windowWidth, &windowHeigth);
            GetTimer(lastTime, deltaTime);
            actionTimer += deltaTime;
            fpsTimer += deltaTime;
            fps++;
            if (fpsTimer >= 1) {
                fpsTimer = 0;
                lastFps = std::to_string(fps);
                fps = 0;
            }
            camera.UpdateFrustum();
            font.RenderText(fontShader, lastFps, 0.5, 1100, 2, Vector3(1, 0.2, 0.2));
            
            //cubeObj.drawMeshInstance(window, camera, objects, compute);
			test.LoadChunk();
            test.ChunkSetup();
			
            test.ChunkVisibility(&camera);
            //Vector3 camPos = camera.GetPosition() / 16;
            //std::cout << camPos << std::endl;
            camera.RegisterKeyboardInput(window);
            camera.RegisterMouseInput(window);
            
            glfwSwapBuffers(window);
            glfwPollEvents();

			if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS) {	
				for (int j = 0; j < 10; j++) {
					tab.checkAround((start.y + 10 + ha) / 16, (start.y + j) / 16);
					chunk ***monoCy = tab.chunkToRet(start.x + 10 + ha, start.y + j);
					test.loadNewChunk(monoCy, 10 + ha, j);
				}
				ha += 1;
				std::cout << "test\n";
			}
    }
    glfwTerminate();
    return 0;
}
