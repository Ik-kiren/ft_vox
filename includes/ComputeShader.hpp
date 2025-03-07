#pragma once
#include "./Scop.hpp"

class ComputeShader
{
public:
    unsigned int ID;

    ComputeShader(const char *str);
    ~ComputeShader();

    void checkShaderCompile(GLuint shader, const std::string name);
    void checkProgramCompile(GLuint programID);
};