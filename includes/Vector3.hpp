#pragma once
#include <ostream>

class Vector4;

class Vector3 {
 public:
    float x;
    float y;
    float z;


    Vector3();
    Vector3(float x, float y, float z);
    Vector3(float f);
    Vector3(Vector3 *vec);
    Vector3(const Vector3 &vec);
    Vector3(const Vector4 &vec);
    ~Vector3();

    Vector3 Inverse();

    float   &operator[](int index);
    Vector3 operator*(float rhs);
    Vector3 operator*(Vector3 rhs);
    Vector3 &operator/(float rhs);
    Vector3 &operator/(Vector3 rhs);
    Vector3 operator-(Vector3 rhs);
    Vector3 operator+(Vector3 rhs);
    Vector3 &operator=(Vector3 const &rhs);
};

std::ostream &operator<<(std::ostream &out, Vector3 &rhs);
