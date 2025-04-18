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
    Vector3 Round();
    Vector3 Trunc();

    float   &operator[](int index);
    Vector3 operator*(float rhs);
    Vector3 operator*(Vector3 rhs);
    Vector3 operator/(float rhs);
    Vector3 operator/(Vector3 rhs);
    Vector3 operator-(Vector3 rhs);
    Vector3 operator+(Vector3 rhs);
    Vector3 &operator=(Vector3 const &rhs);
    bool operator==(const Vector3 &other) const;
};
bool operator!=(Vector3 const &lhs, Vector3 const &rhs);

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
