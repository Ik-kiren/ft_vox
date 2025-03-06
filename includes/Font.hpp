#pragma once
#include <map>
#include <iostream>
#include "./Shader.hpp"
#include "./Vector3.hpp"

struct Character;

class Font {
 private:
    unsigned int VAO;
    unsigned int VBO;

    std::map<unsigned char, Character> characters;

 public:
    Font(/* args */);
    ~Font();

    void RenderText(Shader &s, std::string text, float x, float y, float scale, Vector3 color);
};
