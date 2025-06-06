#pragma once
#include <ostream>
#include "./Vector3i.hpp"

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
    Vector3 Round();
    Vector3 Trunc();
    float   Distance(Vector3 vec);
    Vector3 ChunkNormalize();
    Vector3 Left();
    Vector3 Right();
    Vector3 Front();
    Vector3 Back();
    Vector3 Up();
    Vector3 Bottom();

    float   &operator[](int index);
    Vector3 operator*(float rhs);
    Vector3 operator*(Vector3 rhs);
    Vector3 operator/(float rhs);
    Vector3 operator/(Vector3 rhs);
    Vector3 operator-(Vector3 rhs);
    Vector3 operator+(Vector3 rhs);
    Vector3 operator+(Vector3i rhs);
    Vector3 operator%(int rhs);
    Vector3 &operator=(Vector3 const &rhs);
    bool operator<(const Vector3 &rhs);
    bool operator>(const Vector3 &rhs);
    bool operator==(const Vector3 &other) const;
};

bool operator!=(Vector3 const &lhs, Vector3 const &rhs);
Vector3 operator/(float lhs, Vector3 rhs);

std::ostream &operator<<(std::ostream &out, Vector3 &rhs);

template <>
struct std::hash<Vector3>
{
  std::size_t operator()(const Vector3& k) const {
    using std::size_t;
    using std::hash;
    using std::string;

    return ((hash<float>()(k.x)
             ^ (hash<float>()(k.y) << 1)) >> 1)
             ^ (hash<float>()(k.z) << 1);
  }
};
