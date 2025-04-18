#pragma once
#include <cmath>
#include "./Vector3.hpp"
#include "./Matrix4.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "./BSphere.hpp"

class Camera {
 private:
    Vector3 position;
    Vector3 front;
    Vector3 up;
    Vector3 right;
    Vector3 worldUp;

    Frustum frustum;
    Matrix4 projectionMat;

    float lastPosX;
    float lastPosY;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    float aspect;
    float fovY;
    float zNear;
    float zFar;

    void setCameraVectors();
    void CreateFrustum(float aspect, float fovY, float zNear, float zFar);

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
    Matrix4 GetProjectionMat();

    void RegisterMouseInput(GLFWwindow *window);
    void RegisterKeyboardInput(GLFWwindow *window);
    void UpdateFrustum();

    Vector3 GetChunkPos();
    
    bool InsideFrustum(BSphere bsphere);
    bool InsideFrustum(AABB aabb);

    Camera &operator=(const Camera &rhs);
};
