#pragma once
#include <vector>
#include "./Matrix4.hpp"

class MatrixStack {

 private:
    std::vector<Matrix4> matrixStack;

 public:
    MatrixStack();
    ~MatrixStack();

    void Push();
    void Pop();
    void translate(Vector3 vec);
    void translate(Vector3 vec, bool local);
    void rotate(Vector3 axis);
    void rotate(float radians, Vector3 axis);
    void rotate(float radians, Vector3 axis, bool local);
    void scale(float scale);
    void scale(Vector3 vec);
    void scale(Vector3 vec, bool local);
    Matrix4 GetMatrix();
    void SetMatrix(Matrix4 mat);
};
