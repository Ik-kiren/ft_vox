#include <cmath>
#include <iostream>
#include <string>
#include <stack>
#include "../includes/Scop.hpp"
#include "../includes/Camera.hpp"
#include "../includes/Mesh.hpp"
#include "../includes/Object.hpp"
#include "../includes/MatrixStack.hpp"
#include "../includes/Animation.hpp"
#include "../includes/Button.hpp"


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
    glEnable(GL_CULL_FACE);

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
    GLFWwindow *window;
    window = InitGLFW();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Shader cubeShader = Shader("./shaders/VertexShader.shader", "./shaders/FragmentShader.shader");
    Shader fontShader = Shader("./shaders/fontVS.shader", "./shaders/fontFS.shader");
    ComputeShader compute = ComputeShader("./shaders/compute.shader");

    Mesh cubeMesh = Mesh("./objects/DirtCube.obj");  

    Camera camera = Camera(Vector3(15, 0, 0), Vector3(0, 1, 0));

    Object cubeObj = Object(cubeShader, &cubeMesh, Vector4(1, 1, 1, 1));

    Font font = Font();

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

    std::vector<Object> map;
    ssboObject objects;
    int indexes = 0;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                map.push_back(Object(cubeShader, &cubeMesh));
                map.back().translate(Vector3(-3 * k, -3 * i, -3 * j));
                objects.instances[indexes].bbox = map.back().getSphere();
                objects.instances[indexes].matrix = *map.back().getModel();
            }
        }
    }
    objects.atomicData = 0;
    


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
            font.RenderText(fontShader, lastFps, 0.5, 1100, 2, Vector3(1, 0.2, 0.2));
            Vector3 camPos = camera.GetPosition();
            
            cubeObj.drawMeshInstance(window, camera, map, objects, compute);

            camera.RegisterKeyboardInput(window);
            camera.RegisterMouseInput(window);

            glfwSwapBuffers(window);
            glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
