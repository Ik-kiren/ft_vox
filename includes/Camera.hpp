#pragma once
#include <cmath>
#include "./Vector3.hpp"
#include "./Matrix4.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "./BSphere.hpp"
#include "Player.hpp"

class Camera {
 private:
    float fov;
    float nearPlane;
    float farPlane;

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
	float speedFps;
    float sensitivity;

    float aspect;
    float fovY;
    float zNear;
    float zFar;

    Player *player;

    void setCameraVectors();
    void CreateFrustum(float aspect, float fovY, float zNear, float zFar);

 public:
    Camera();
    Camera(Vector3 cameraPos, Vector3 up);
    ~Camera();

    Vector3 GetPosition();
    Vector3 GetDirection();
    Vector3 GetUp();
    Vector3 GetFront();
    Vector3 GetRight();
    Matrix4 GetViewMatrix();
    Matrix4 GetViewMatrix(Vector3 lookatpos);
    Matrix4 GetProjectionMat();
	void	SetSpeedFps(float speed);
	void	setYfromPlayer(Player &player);

    void RegisterMouseInput(GLFWwindow *window);
    void RegisterKeyboardInput(GLFWwindow *window);
    void RegisterKeyboardInput(GLFWwindow *window, double deltaTime);
    void RegisterKeyboardInput(GLFWwindow *window, int fps);
    void UpdateFrustum();
    bool AABBInterstect(AABB &aabb);

    Vector3 GetChunkPos();
    Vector3 PosToChunkPos(Vector3 pos);
    
    bool InsideFrustum(BSphere &bsphere);
    bool InsideFrustum(AABB &aabb);

    Camera &operator=(const Camera &rhs);
};
