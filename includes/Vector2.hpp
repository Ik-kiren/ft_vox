#pragma once
#include <ostream>

class Vector2 {
 public:
    float x;
    float y;


    Vector2();
    Vector2(float x, float y);
    Vector2(Vector2 *vec);
    Vector2(const Vector2 &vec);
    ~Vector2();

    float   &operator[](int index);
    Vector2 operator*(float rhs);
    Vector2 operator*(Vector2 rhs);
    Vector2 &operator/(float rhs);
    Vector2 operator-(Vector2 rhs);
    Vector2 operator+(Vector2 rhs);
    Vector2 &operator=(Vector2 const &rhs);
};

std::ostream &operator<<(std::ostream &out, Vector2 &rhs);
