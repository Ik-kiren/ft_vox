#include "../includes/Camera.hpp"
#include "../includes/Scop.hpp"

Camera::Camera() {}

Camera::Camera(Vector3 cameraPos, Vector3 up) : position(cameraPos), worldUp(up) {
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 0.3f;
    sensitivity = 0.0001f;
    setCameraVectors();
    lastPosX = cameraPos.x;
    lastPosY = cameraPos.y;
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

    if (pitch >= 89.0f)
        pitch = 89.0f;
    else if (pitch <= -89.0f)
        pitch = -89.0f;
    Vector3 tmpFront = Vector3();
    tmpFront.x = cos(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));
    tmpFront.y = sin(pitch * (M_PI / 180));
    tmpFront.z = sin(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));
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

void Camera::setCameraVectors() {
    Vector3 tmpFront = Vector3();
    tmpFront.x = cos(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));
    tmpFront.y = sin(pitch * (M_PI / 180));
    tmpFront.z = sin(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));
    front = normalized(tmpFront);

    right = normalized(cross(front, worldUp));
    up = normalized(cross(right, front));
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
