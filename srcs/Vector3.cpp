#include "../includes/Vector3.hpp"
#include <vector>
#include <string>
#include <math.h>
#include "../includes/Vector4.hpp"

Vector3::Vector3() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3::Vector3(float f) {
    this->x = f;
    this->y = f;
    this->z = f;
}

Vector3::Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(Vector3 *vec) {
    this->x = vec->x;
    this->y = vec->y;
    this->z = vec->z;
}

Vector3::Vector3(const Vector3 &vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

Vector3::Vector3(const Vector4 &vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

Vector3::~Vector3() {}

Vector3 Vector3::Inverse() {
    return Vector3(this->x * -1, this->y * -1, this->z * -1);
}

Vector3 Vector3::Round() {
    return Vector3(round(this->x), round(this->y), round(this->z));
}

float &Vector3::operator[](int index) {
    if (index < 0 || index > 2) {
        std::string str = "Vector3: " + std::to_string(index) + " out of range";
        throw std::out_of_range(str);
    }
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    return z;
}

Vector3 Vector3::operator*(float rhs) {
    return Vector3(x * rhs, y * rhs, z * rhs);
}

Vector3 Vector3::operator*(Vector3 rhs) {
    return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
}


Vector3 Vector3::operator/(float rhs) {
    if (rhs == 0)
        return *this;
    return Vector3(x / rhs, y / rhs, z / rhs);
}

Vector3 Vector3::operator/(Vector3 rhs) {
    if (rhs.x == 0 || rhs.y == 0 || rhs.z == 0)
        return *this;
    return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
}

Vector3 Vector3::operator-(Vector3 rhs) {
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator+(Vector3 rhs) {
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 &Vector3::operator=(Vector3 const &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    return *this;
}

bool operator!=(Vector3 const &lhs, Vector3 const &rhs){
    return (lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z);
}

std::ostream &operator<<(std::ostream &out, Vector3 &rhs) {
    out << "<";
    for (int i = 0; i < 3; i++) {
        out << rhs[i] << ", ";
    }
    out << ">";
    return out;
}

bool Vector3::operator==(const Vector3 &other) const
  { return (this->x == other.x
            && this->y == other.y
            && this->z == other.z);
  }