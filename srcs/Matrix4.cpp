#include <stdexcept>
#include <string>
#include "../includes/Matrix4.hpp"

//SIMD

void Matrix4::MatrixArrayInit() {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            matrix_array[i * 4 + j] = matrix[i][j];
        }
    }
}

void Matrix4::DataInit() {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            matrix_data[i][j] = matrix[i][j];
        }
    }
}

Matrix4::Matrix4() {}

Matrix4::Matrix4(const Matrix4 &mat) {
    matrix[0] = mat[0];
    matrix[1] = mat[1];
    matrix[2] = mat[2];
    matrix[3] = mat[3];
    MatrixArrayInit();
}

Matrix4::Matrix4(float nbr) {
    matrix[0] = Vector4(nbr, 0, 0, 0);
    matrix[1] = Vector4(0, nbr, 0, 0);
    matrix[2] = Vector4(0, 0, nbr, 0);
    matrix[3] = Vector4(0, 0, 0, nbr);
    MatrixArrayInit();
}

Matrix4::Matrix4(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4) {
    matrix[0] = Vector4(v1);
    matrix[1] = Vector4(v2);
    matrix[2] = Vector4(v3);
    matrix[3] = Vector4(v4);
    MatrixArrayInit();
}

Matrix4::Matrix4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4) {
    matrix[0] = v1;
    matrix[1] = v2;
    matrix[2] = v3;
    matrix[3] = v4;
    MatrixArrayInit();
}

Matrix4::~Matrix4() {}

float *Matrix4::get_array() {
    MatrixArrayInit();
    return matrix_array;
}

Vector4& Matrix4::operator[](int index) {
    if (index < 0 || index > 3) {
        std::string str = "matrix4 ";
        str  = str + std::to_string(index) + "out of range";
        throw std::out_of_range(str);
    }
    return matrix[index];
}

const Vector4& Matrix4::operator[](int index) const {
    if (index < 0 || index > 3) {
        std::string str = "matrix4 ";
        str  = str + std::to_string(index) + "out of range";
        throw std::out_of_range(str);
    }
    return matrix[index];
}

Matrix4 Matrix4::operator*(Matrix4 rhs) {
    Matrix4 mat = Matrix4(1.0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = matrix[i][0] * rhs[0][j] + matrix[i][1] * rhs[1][j] +
                matrix[i][2] * rhs[2][j] + matrix[i][3] * rhs[3][j];
        }
    }
    return mat;
}

Vector4 Matrix4::operator*(Vector4 rhs) {
    Vector4 vec = Vector4(0, 0, 0, 0);
    for (int i = 0; i < 4; i++) {
        vec[i] = matrix[0][i] * rhs[i] + matrix[1][i] * rhs[i] + matrix[2][i] * rhs[i] +
            matrix[3][i] * rhs[i];
    }
    return vec;
}

Matrix4 &Matrix4::operator=(Matrix4 const &rhs) {
    for (size_t i = 0; i < 4; i++) {
        this->matrix[i] = rhs.matrix[i];
    }
    this->MatrixArrayInit();
    return *this;
}

std::ostream &operator<<(std::ostream &out, Matrix4 &rhs) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << rhs[i][j] << " | ";
        }
        out << std::endl;
    }
    return out;
}
