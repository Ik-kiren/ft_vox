#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "./stb_image.h"
#include "./Matrix4.hpp"
#include "./Vector3.hpp"
#include "./Vector2.hpp"
#include "./Font.hpp"
#include "./BSphere.hpp"

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    Vector2   Size;      // Size of glyph
    Vector2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

// ScopMaths.cpp

Vector3 cross(Vector3 vec1, Vector3 vec2);
float magnitude(const Vector3 &vec);
Vector3 normalized(Vector3 vec);
float Dot(Vector3 vec1, Vector3 vec2);
Matrix4 Translate(Matrix4 mat, Vector3 vec);
Matrix4 Translate(Matrix4 mat, Vector3 vec, bool local);
Matrix4 Transposed(Matrix4 mat);
Matrix4 lookAt(Vector3 position, Vector3 target, Vector3 upVector);
Matrix4 Rotate(Matrix4 mat, float radians, Vector3 axis);
Matrix4 Rotate(Matrix4 mat, float radians, Vector3 axis, bool local);
Matrix4 Perspective(float fov, float aspectRatio, float near, float far);
Matrix4 Orthographique(float left, float right, float bot, float top, float near, float far);
Matrix4 Scale(Matrix4 mat, float scale);
Matrix4 Scale(Matrix4 mat, Vector3 vec);
Matrix4 Scale(Matrix4 mat, Vector3 vec, bool local);
double lerp(double a, double b, double t);

