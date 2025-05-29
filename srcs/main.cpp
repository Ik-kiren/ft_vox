#include <cmath>
#include <iostream>
#include <string>
#include "../includes/ChunkManager.hpp"
#include "../includes/Renderer.hpp"
#include "../includes/ShadowMap.hpp"
#include "../includes/mapGP.hpp"
#include "../includes/GameManager.hpp"
#include "../includes/Constants.hpp"
#include <chrono>

int seed = 2039412743;

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
    window = glfwCreateWindow(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT, "window", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return window;
}

int main(void) {
    srand(time(NULL));
	seed = std::rand();
    Renderer renderer;
    mapGP tab(32, 32);
    Player	player(0, 128, 0);
    ChunkManager test(&renderer, &tab, &player);
	test.Init();

    GLFWwindow *window = InitGLFW();

    GameManager gameManager = GameManager(&renderer, &tab, &player, &test, window);

    while ((glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0)) {
            glClearColor(0.0f, 0.45f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            gameManager.Update();

            glfwSwapBuffers(window);
			glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
