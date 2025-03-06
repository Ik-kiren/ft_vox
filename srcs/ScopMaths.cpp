#include <cmath>
#include <limits>
#include "../includes/Scop.hpp"
#include "../includes/Camera.hpp"
#include "../includes/ScopMaths.hpp"

Vector3 cross(Vector3 vec1, Vector3 vec2) {
    return Vector3(
        vec1.y * vec2.z - vec2.y * vec1.z,
        vec1.z * vec2.x - vec2.z * vec1.x,
        vec1.x * vec2.y - vec2.x * vec1.y);
}

float Dot(Vector3 vec1, Vector3 vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

float magnitude(const Vector3 &vec) {
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

Vector3 normalized(Vector3 vec) {
    return vec / magnitude(vec);
}

Matrix4 Translate(Matrix4 mat, Vector3 vec) {
    Matrix4 translationMat = Matrix4(1.0);
    translationMat[3] = Vector4(vec);
    return translationMat * mat;
}

Matrix4 Translate(Matrix4 mat, Vector3 vec, bool local) {
    Matrix4 translationMat = Matrix4(1.0);
    translationMat[3] = Vector4(vec);
    if (local)
        return translationMat * mat;
    else
        return mat * translationMat;
}

Matrix4 Transposed(Matrix4 mat) {
    Matrix4 newMat = Matrix4(
        Vector4(mat[0][0], mat[1][0], mat[2][0], mat[3][0]),
        Vector4(mat[0][1], mat[1][1], mat[2][1], mat[3][1]),
        Vector4(mat[0][2], mat[1][2], mat[2][2], mat[3][2]),
        Vector4(mat[0][3], mat[1][3], mat[2][3], mat[3][3]));
    return newMat;
}

Matrix4 lookAt(Vector3 position, Vector3 target, Vector3 upVector) {
    Vector3 direction = normalized(target - position);
    Vector3 right = normalized(cross(direction, upVector));
    Vector3 up = cross(right, direction);
    Matrix4 lookAtMatrix = Matrix4(
        Vector4(right, 0.0f),
        Vector4(up, 0.0f),
        Vector4(direction.x * -1, direction.y * -1, direction.z * -1, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    lookAtMatrix[0][3] = Dot(position, right) * -1;
    lookAtMatrix[1][3] = Dot(position, up) * -1;
    lookAtMatrix[2][3] = Dot(position, direction);
    return Transposed(lookAtMatrix);
}

Matrix4 Rotate(Matrix4 mat, float radians, Vector3 axis) {
    Matrix4 rotateMat = Matrix4(
        Vector4(cos(radians) + powf(axis[0], 2.0) * (1 - cos(radians)),
            axis[0] * axis[1] * (1 - cos(radians)) + axis[2] * sin(radians),
            axis[0] * axis[2] * (1 - cos(radians)) - axis[1] * sin(radians), 0),
        Vector4(axis[1] * axis[0] * (1 - cos(radians)) - axis[2] * sin(radians),
            cos(radians) + powf(axis[1], 2.0) * (1 - cos(radians)),
            axis[1] * axis[2] * (1 - cos(radians)) + axis[0] * sin(radians), 0),
        Vector4(axis[2] * axis[0] * (1 - cos(radians)) + axis[1] * sin(radians),
            axis[2] * axis[1] * (1 - cos(radians)) - axis[0] * sin(radians),
            cos(radians) + powf(axis[2], 2.0) * (1 - cos(radians)), 0),
        Vector4(0, 0, 0, 1));
    return rotateMat * mat;
}

Matrix4 Rotate(Matrix4 mat, float radians, Vector3 axis, bool local) {
    Matrix4 rotateMat = Matrix4(
        Vector4(cos(radians) + powf(axis[0], 2.0) * (1 - cos(radians)),
            axis[0] * axis[1] * (1 - cos(radians)) + axis[2] * sin(radians),
            axis[0] * axis[2] * (1 - cos(radians)) - axis[1] * sin(radians), 0),
        Vector4(axis[1] * axis[0] * (1 - cos(radians)) - axis[2] * sin(radians),
            cos(radians) + powf(axis[1], 2.0) * (1 - cos(radians)),
            axis[1] * axis[2] * (1 - cos(radians)) + axis[0] * sin(radians), 0),
        Vector4(axis[2] * axis[0] * (1 - cos(radians)) + axis[1] * sin(radians),
            axis[2] * axis[1] * (1 - cos(radians)) - axis[0] * sin(radians),
            cos(radians) + powf(axis[2], 2.0) * (1 - cos(radians)), 0),
        Vector4(0, 0, 0, 1));
    if (local)
        return rotateMat * mat;
    else
        return mat * rotateMat;
}

Matrix4 Scale(Matrix4 mat, float scale) {
    Matrix4 scaleMatrix = Matrix4(scale);
    scaleMatrix[3][3] = 1;
    Matrix4 tmp = scaleMatrix * mat;
    return scaleMatrix * mat;
}

Matrix4 Scale(Matrix4 mat, Vector3 vec) {
    Matrix4 scaleMatrix = Matrix4(1);
    scaleMatrix[0][0] = vec.x;
    scaleMatrix[1][1] = vec.y;
    scaleMatrix[2][2] = vec.z;
    return scaleMatrix * mat;
}

Matrix4 Scale(Matrix4 mat, Vector3 vec, bool local) {
    Matrix4 scaleMatrix = Matrix4(1);
    scaleMatrix[0][0] = vec.x;
    scaleMatrix[1][1] = vec.y;
    scaleMatrix[2][2] = vec.z;
    if (local)
        return scaleMatrix * mat;
    else
        return mat * scaleMatrix;
}

Matrix4 Perspective(float fov, float aspectRatio, float near, float far) {
    const float DEG2RAD = acos(-1.0f) / 180;
    float tangent = tan(fov / 2 * DEG2RAD);
    float top = near * tangent;
    float right = top * aspectRatio;

    Matrix4 matrix = Matrix4(
        Vector4(near / right, 0, 0, 0),
        Vector4(0, near / top, 0, 0),
        Vector4(0, 0, -(far + near) / (far - near), - 1),
        Vector4(0, 0, -(2 * far * near) / (far - near), 0));
    return matrix;
}

Matrix4 Orthographique(float left, float right, float bot, float top, float near, float far) {
    Matrix4 matrix = Matrix4(
        Vector4(2 / (right - left), 0, 0, 0),
        Vector4(0, 2 / (top - bot), 0, 0),
        Vector4(0, 0, -2 / (far - near), 0),
        Vector4(-((right + left) / (right - left)), -((top + bot) / (top - bot)), -((far + near) / (far - near)), 1));
    return matrix;
}


double lerp(double a, double b, double t) {
    return a + t * (b - a);
}

