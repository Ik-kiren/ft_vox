#include "../includes/Vector3i.hpp"
#include <vector>
#include <string>
#include "../includes/Vector3.hpp"

Vector3i::Vector3i() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3i::Vector3i(int f) {
    this->x = f;
    this->y = f;
    this->z = f;
}

Vector3i::Vector3i(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3i::Vector3i(Vector3 vec) {
    this->x = static_cast<int>(vec.x);
    this->y = static_cast<int>(vec.y);
    this->z = static_cast<int>(vec.z);
}

Vector3i::Vector3i(Vector3i *vec) {
    this->x = vec->x;
    this->y = vec->y;
    this->z = vec->z;
}

Vector3i::Vector3i(const Vector3i &vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

Vector3i::~Vector3i() {}

Vector3i Vector3i::Inverse() {
    return Vector3i(x * -1, y * -1, z * -1);
}

int &Vector3i::operator[](int index) {
    if (index < 0 || index > 2) {
        std::string str = "Vector3i: " + std::to_string(index) + " out of range";
        throw std::out_of_range(str);
    }
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    return z;
}

Vector3i Vector3i::operator*(int rhs) {
    return Vector3i(x * rhs, y * rhs, z * rhs);
}

Vector3i Vector3i::operator*(Vector3i rhs) {
    return Vector3i(x * rhs.x, y * rhs.y, z * rhs.z);
}


Vector3i Vector3i::operator/(int rhs) {
    if (rhs == 0)
        return *this;
    return Vector3i(x / rhs, y / rhs, z / rhs);
}

Vector3i Vector3i::operator/(Vector3i rhs) {
    if (rhs.x == 0 || rhs.y == 0 || rhs.z == 0)
        return *this;
    return Vector3i(x / rhs.x, y / rhs.y, z / rhs.z);
}

Vector3i Vector3i::operator-(Vector3i rhs) {
    return Vector3i(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3i Vector3i::operator+(Vector3i rhs) {
    return Vector3i(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3i &Vector3i::operator=(Vector3i const &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    return *this;
}

bool operator!=(Vector3i const &lhs, Vector3i const &rhs){
    return (lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z);
}

std::ostream &operator<<(std::ostream &out, Vector3i &rhs) {
    out << "<";
    for (int i = 0; i < 3; i++) {
        out << rhs[i] << ", ";
    }
    out << ">";
    return out;
}

bool Vector3i::operator==(const Vector3i &other) const
  { return (this->x == other.x
            && this->y == other.y
            && this->z == other.z);
  }