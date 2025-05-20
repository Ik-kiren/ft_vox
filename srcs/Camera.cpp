#include "../includes/Camera.hpp"
#include "../includes/Scop.hpp"
#include "../includes/BSphere.hpp"
#include "../includes/ScopMaths.hpp"

Camera::Camera() {}

Camera::Camera(Vector3 cameraPos, Vector3 up) : position(cameraPos), worldUp(up) {
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 0.3f;
	speedFps = 6.0f;
    sensitivity = 0.0001f;
    setCameraVectors();
    lastPosX = cameraPos.x;
    lastPosY = cameraPos.y;
    CreateFrustum((1920.0f / 1200.0f), 80.0f, 0.1f, 160.0f);
    projectionMat = Perspective(80.0f, (1920.0f / 1200.0f), 0.1f, 160.0f);
}

Camera::~Camera() {}

Vector3 Camera::GetPosition(){
    return this->position;
}

Vector3 Camera::GetDirection(){
    return this->front;
}

Vector3 Camera::GetUp(){
    return this->up;
}

Vector3 Camera::GetFront() {
    return this->front;
}

Vector3 Camera::GetRight() {
    return this->right;
}


Matrix4 Camera::GetViewMatrix() {
    return lookAt(position, position + front, up);
}

Matrix4 Camera::GetViewMatrix(Vector3 lookatpos) {
    return lookAt(position, lookatpos, up);
}

Matrix4 Camera::GetProjectionMat() {
    return projectionMat;
}

void	Camera::SetSpeedFps(float speed) {
	speedFps += speed;
	if (speedFps > 20)
		speedFps = 20;
	if (speedFps < 1)
		speedFps = 1;
}

void	Camera::setYfromPlayer(Player &player) {
	this->position = Vector3(player._pos.x, player._pos.y + 3, player._pos.z);
}

void Camera::CreateFrustum(float aspect, float fovY, float zNear, float zFar) {
    this->aspect = aspect;
    this->fovY = fovY;
    this->zNear = zNear;
    this->zFar = zFar;
    const float DEG2RAD = M_PI / 180;
    const float halfVSide = zFar * tanf(fovY * DEG2RAD * 0.5f);
    const float halfHSide = halfVSide * aspect;
    Vector3 frontMultFar = this->GetFront() * zFar;
    
    this->frustum.nearFace = CreatePlane(this->GetPosition() + (this->GetFront() * zNear), this->GetFront());
    this->frustum.farFace = CreatePlane(this->GetPosition() + frontMultFar, this->GetFront() * -1);
    this->frustum.rightFace = CreatePlane(this->GetPosition(), cross(frontMultFar - (this->GetRight() * halfHSide), this->GetUp()));
    this->frustum.leftFace = CreatePlane(this->GetPosition(), cross(this->GetUp(), frontMultFar + this->GetRight()  * halfHSide));
    this->frustum.topFace = CreatePlane(this->GetPosition(), cross(this->GetRight(), frontMultFar - this->GetUp() * halfVSide));
    this->frustum.bottomFace = CreatePlane(this->GetPosition(), cross(frontMultFar + this->GetUp() * halfVSide, this->GetRight()));
}

void Camera::UpdateFrustum() {
    const float DEG2RAD = M_PI / 180;
    const float halfVSide = zFar * tanf(fovY * DEG2RAD * 0.5f);
    const float halfHSide = halfVSide * aspect;
    Vector3 frontMultFar = this->GetFront() * zFar;
    
    this->frustum.nearFace = CreatePlane(this->GetPosition() + (this->GetFront() * zNear), this->GetFront());
    this->frustum.farFace = CreatePlane(this->GetPosition() + frontMultFar, this->GetFront() * -1);
    this->frustum.rightFace = CreatePlane(this->GetPosition(), cross(frontMultFar - (this->GetRight() * halfHSide), this->GetUp()));
    this->frustum.leftFace = CreatePlane(this->GetPosition(), cross(this->GetUp(), frontMultFar + this->GetRight()  * halfHSide));
    this->frustum.topFace = CreatePlane(this->GetPosition(), cross(this->GetRight(), frontMultFar - this->GetUp() * halfVSide));
    this->frustum.bottomFace = CreatePlane(this->GetPosition(), cross(frontMultFar+ this->GetUp() * halfVSide, this->GetRight()));
}

bool Camera::InsideFrustum(BSphere bsphere) {
    return frustum.IsInFrustum(bsphere);
}

bool Camera::InsideFrustum(AABB aabb) {
    return frustum.IsInFrustum(aabb);
}

void Camera::RegisterMouseInput(GLFWwindow *window) {
    double xpos, ypos;
    float xoffset, yoffset;

    glfwGetCursorPos(window, &xpos, &ypos);
    xoffset = xpos - lastPosX;
    yoffset = lastPosY - ypos;
    lastPosX = xpos;
    lastPosY = ypos;

    const float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;
    
    Vector3 tmpFront = Vector3();
    tmpFront.x = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    tmpFront.y = sin(pitch * (M_PI / 180));
    tmpFront.z = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));

    front = normalized(tmpFront);
    right = normalized(cross(front, worldUp));
    up = normalized(cross(right, front));
}

void Camera::RegisterKeyboardInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position = position + normalized(cross(front, Vector3(0, 1, 0))) * speed;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position = position - normalized(cross(front, Vector3(0, 1, 0))) * speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position = position + front * speed;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position = position - front * speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position = position + up * speed;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        position = position - up * speed;
}

void Camera::RegisterKeyboardInput(GLFWwindow *window, double deltaTime) {
	speed = speedFps * deltaTime;
	Vector3 dir;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        dir = dir + normalized(cross(front, Vector3(0, 1, 0)));
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        dir = dir - normalized(cross(front, Vector3(0, 1, 0)));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dir = dir + front;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dir = dir - front;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		dir = dir + up;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		dir = dir - up;
	dir = normalized(dir);
	position = position + dir * speed;
}

void Camera::RegisterKeyboardInput(GLFWwindow *window, int fps) {
	speed = speedFps / (float)fps;
	Vector3 dir;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        dir = dir + normalized(cross(front, Vector3(0, 1, 0)));
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        dir = dir - normalized(cross(front, Vector3(0, 1, 0)));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dir = dir + front;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dir = dir - front;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		dir = dir + up;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		dir = dir - up;
	dir = normalized(dir);
	position = position + dir * speed;
}

void Camera::setCameraVectors() {
    Vector3 tmpFront = Vector3();
    tmpFront.x = cos(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));
    tmpFront.y = sin(pitch * (M_PI / 180));
    tmpFront.z = sin(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));
    front = normalized(tmpFront);

    right = normalized(cross(front, worldUp));
    up = normalized(cross(right, front));
}

Vector3 Camera::GetChunkPos() {
    Vector3 tmp(0.0);
    tmp = this->GetPosition();
    if (tmp.x < 0)
        tmp.x -= 16;
    if (tmp.z < 0)
        tmp.z -= 16;
    return (tmp / 16).Trunc();
}

Camera &Camera::operator=(const Camera &rhs) {
    this->position = rhs.position;
    this->front = rhs.front;
    this->up = rhs.up;
    this->right = rhs.right;
    this->worldUp = rhs.worldUp;
    this->lastPosX = rhs.lastPosX;
    this->lastPosY = rhs.lastPosY;
    this->yaw = rhs.yaw;
    this->pitch = rhs.pitch;
    this->speed = rhs.speed;
    this->sensitivity = rhs.sensitivity;
    return *this;
}
