#pragma once
#include <ostream>
#include "./Vector3.hpp"

class Vector4 {
 public:
    float x;
    float y;
    float z;
    float w;

    static const Vector4 GREEN;
    static const Vector4 TGREEN;
    static const Vector4 RED;
    static const Vector4 TRED;

    Vector4(); 
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector3 &vec);
    Vector4(Vector3 vec, float nbr);
    Vector4(const Vector4 &vec);
    ~Vector4();

    Vector4 &operator=(Vector4 const &rhs);
    float &operator[](int index);
    Vector4 &operator+(Vector4 rhs);
    Vector4 &operator*(float rhs);
};

std::ostream &operator<<(std::ostream &out, Vector4 &rhs);
