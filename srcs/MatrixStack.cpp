#include <cmath>
#include "../includes/MatrixStack.hpp"
#include "../includes/Scop.hpp"

MatrixStack::MatrixStack() {
    matrixStack.push_back(Matrix4(1));
}

MatrixStack::~MatrixStack() {
}

void MatrixStack::Push() {
    matrixStack.push_back(matrixStack.back());
}

void MatrixStack::Pop() {
    matrixStack.pop_back();
}

void MatrixStack::translate(Vector3 vec) {
    Matrix4 &last = matrixStack.back();
    last = Translate(last, vec);
}

void MatrixStack::translate(Vector3 vec, bool local) {
    Matrix4 &last = matrixStack.back();
    last = Translate(last, vec, local);
}

void MatrixStack::rotate(Vector3 axis) {
    Matrix4 &last = matrixStack.back();
    last = Rotate(last, 5 * M_PI / 4, axis);
}

void MatrixStack::rotate(float radians, Vector3 axis) {
    Matrix4 &last = matrixStack.back();
    last = Rotate(last, radians * M_PI, axis);
}

void MatrixStack::rotate(float radians, Vector3 axis, bool local) {
    Matrix4 &last = matrixStack.back();
    last = Rotate(last, radians * M_PI, axis, local);
}

void MatrixStack::scale(float scale) {
    Matrix4 &last = matrixStack.back();
    last = Scale(last, scale);
}

void MatrixStack::scale(Vector3 vec) {
    Matrix4 &last = matrixStack.back();
    last = Scale(last, vec);
}

void MatrixStack::scale(Vector3 vec, bool local) {
    Matrix4 &last = matrixStack.back();
    last = Scale(last, vec, local);
}

Matrix4 MatrixStack::GetMatrix() {
    return matrixStack.back();
}

void MatrixStack::SetMatrix(Matrix4 mat) {
     Matrix4 &last = matrixStack.back();
     last = mat;
}
