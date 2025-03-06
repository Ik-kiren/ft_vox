#pragma once
#include <cmath>
#include "./Vector3.hpp"
#include "./Matrix4.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera {
 private:
    Vector3 position;
    Vector3 front;
    Vector3 up;
    Vector3 right;
    Vector3 worldUp;

    float lastPosX;
    float lastPosY;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    void setCameraVectors();

 public:
    Camera();
    Camera(Vector3 cameraPos, Vector3 up);
    ~Camera();

    Vector3 GetPosition() ;
    Vector3 GetDirection() ;
    Vector3 GetUp() ;
    Vector3 GetFront() ;
    Vector3 GetRight() ;
    Matrix4 GetViewMatrix();
    Matrix4 GetViewMatrix(Vector3 lookatpos);

    void RegisterMouseInput(GLFWwindow *window);
    void RegisterKeyboardInput(GLFWwindow *window);

    Camera &operator=(const Camera &rhs);
};
