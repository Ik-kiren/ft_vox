#include "../includes/Vector2.hpp"
#include <vector>
#include <string>

Vector2::Vector2() {
    this->x = 0;
    this->y = 0;
}

Vector2::Vector2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2::Vector2(Vector2 *vec) {
    this->x = vec->x;
    this->y = vec->y;
}

Vector2::Vector2(const Vector2 &vec) {
    this->x = vec.x;
    this->y = vec.y;
}

Vector2::~Vector2() {}

float &Vector2::operator[](int index) {
    if (index < 0 || index > 1) {
        std::string str = "Vector2: " + std::to_string(index) + " out of range";
        throw std::out_of_range(str);
    }
    if (index == 0)
        return x;
    return y;
}

Vector2 Vector2::operator*(float rhs) {
    return Vector2(x * rhs, y * rhs);
}

Vector2 Vector2::operator*(Vector2 rhs) {
    return Vector2(x * rhs.x, y * rhs.y);
}


Vector2 &Vector2::operator/(float rhs) {
    if (rhs == 0)
        return *this;
    x = x / rhs;
    y = y / rhs;
    return *this;
}

Vector2 Vector2::operator-(Vector2 rhs) {
    return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator+(Vector2 rhs) {
    return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 &Vector2::operator=(Vector2 const &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    return *this;
}

std::ostream &operator<<(std::ostream &out, Vector2 &rhs) {
    out << "<";
    for (int i = 0; i < 2; i++) {
        out << rhs[i] << ", ";
    }
    out << ">" << std::endl;
    return out;
}
