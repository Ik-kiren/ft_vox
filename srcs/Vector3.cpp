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

Vector3 Vector3::Trunc() {
    return Vector3(trunc(this->x), trunc(this->y), trunc(this->z));
}

float   Vector3::Distance(Vector3 vec) {
    return (sqrt(pow(this->x - vec.x, 2) + pow(this->y - vec.y, 2) + pow(this->z - vec.z, 2)));
}

Vector3 Vector3::ChunkNormalize() {
    Vector3 tmp = this;
    if(tmp.x < 0)
        tmp.x = tmp.x -1;
    if(tmp.y < 0)
        tmp.y = tmp.y -1;
    if(tmp.z < 0)
        tmp.z = tmp.z -1;
    return tmp;
}

Vector3 Vector3::Left() {
    return (Vector3(this->x - 1, this->y, this->z));
}

Vector3 Vector3::Right() {
    return (Vector3(this->x + 1, this->y, this->z));
}

Vector3 Vector3::Front() {
    return (Vector3(this->x, this->y, this->z + 1));
}

Vector3 Vector3::Back() {
    return (Vector3(this->x, this->y, this->z - 1));
}

Vector3 Vector3::Up() {
    return (Vector3(this->x, this->y + 1, this->z));
}

Vector3 Vector3::Bottom() {
    return (Vector3(this->x, this->y - 1, this->z));
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

Vector3 operator/(float lhs, Vector3 rhs) {
    if (lhs == 0)
        return rhs;
    return Vector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
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

Vector3 Vector3::operator+(Vector3i rhs) {
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator%(int rhs) {
    return Vector3(static_cast<int>(this->x) % rhs, static_cast<int>(this->y) % rhs, static_cast<int>(this->z) % rhs);
}

Vector3 &Vector3::operator=(Vector3 const &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    return *this;
}

bool Vector3::operator<(const Vector3 &rhs) {
    return (this->x < rhs.x || this->y < rhs.y || this->z < rhs.z);
}

bool Vector3::operator>(const Vector3 &rhs) {
    return (this->x > rhs.x || this->y > rhs.y ||this->z > rhs.z);
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

bool operator==(const Vector3& lhs, const Vector3& rhs) {
    return (lhs.x == rhs.x && lhs.y < rhs.y && lhs.z < rhs.z);
}

bool Vector3::operator==(const Vector3 &other) const
  { return (this->x == other.x
            && this->y == other.y
            && this->z == other.z);
  }