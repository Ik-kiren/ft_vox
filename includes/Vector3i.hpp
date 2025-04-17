#pragma once
#include <ostream>

class Vector3;

class Vector3i {
 public:
    int x;
    int y;
    int z;


    Vector3i();
    Vector3i(int x, int y, int z);
    Vector3i(int f);
    Vector3i(Vector3 vec);
    Vector3i(Vector3i *vec);
    Vector3i(const Vector3i &vec);
    ~Vector3i();

    Vector3i Inverse();

    int   &operator[](int index);
    Vector3i operator*(int rhs);
    Vector3i operator*(Vector3i rhs);
    Vector3i operator/(int rhs);
    Vector3i operator/(Vector3i rhs);
    Vector3i operator-(Vector3i rhs);
    Vector3i operator+(Vector3i rhs);
    Vector3i &operator=(Vector3i const &rhs);
    bool operator==(const Vector3i &other) const;
};
bool operator!=(Vector3i const &lhs, Vector3i const &rhs);

std::ostream &operator<<(std::ostream &out, Vector3i &rhs);

template <>
struct std::hash<Vector3i>
{
  std::size_t operator()(const Vector3i& k) const {
    using std::size_t;
    using std::hash;
    using std::string;

    return ((hash<int>()(k.x)
             ^ (hash<int>()(k.y) << 1)) >> 1)
             ^ (hash<int>()(k.z) << 1);
  }
};