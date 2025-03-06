#include "../includes/Button.hpp"

Button::Button(Vector2 pos, Vector2 size) : pos(pos), size(size) {

}

Button::Button(float x, float y, float width, float heigth) : pos(x, y), size(width, heigth){

}

Button::~Button() {

}

bool Button::Clicked(GLFWwindow *window, double mouseX, double mouseY) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
      (mouseX >= pos.x && mouseX <= pos.x + size.x) && (mouseY >= pos.y && mouseY <= pos.y + size.y))
        return true;
    else
        return false;
}
