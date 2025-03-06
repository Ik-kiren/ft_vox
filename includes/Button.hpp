#pragma once
#include "./Vector2.hpp"
#include "./Scop.hpp"

class Button {
 private:
    Vector2 pos;
    Vector2 size;
 public:
    Button(Vector2 pos, Vector2 size);
    Button(float x, float y, float width, float heigth);
    ~Button();

    bool Clicked(GLFWwindow *window, double mouseX, double mouseY);
};

