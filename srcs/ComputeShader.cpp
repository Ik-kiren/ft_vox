#include "../includes/ComputeShader.hpp"
#include <GL/glew.h>
#include <iostream>

ComputeShader::ComputeShader(const char *computePath) {
    std::string     computeCode;
    std::ifstream   computeFile;
    try {
        computeFile.open(computePath);

        std::stringstream computeStream;
        std::stringstream fragmentStream;

        computeStream << computeFile.rdbuf();

        computeFile.close();

        computeCode = computeStream.str();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        glfwTerminate();
        exit(0);
    }
    const char* vComputeCode = computeCode.c_str();

    unsigned int compute;
    // compute shader
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &vComputeCode, NULL);
    glCompileShader(compute);
    checkShaderCompile(compute, "COMPUTE");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, compute);
    glLinkProgram(ID);
    checkProgramCompile(ID);
}

ComputeShader::~ComputeShader() {

}

void ComputeShader::checkShaderCompile(GLuint shader, const std::string name) {
    int success;
    char logs[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, logs);
        std::cout << "SHADER COMPILER ERROR: " << name << "\n" << logs << std::endl;
        glfwTerminate();
        exit(0);
    }
}

void ComputeShader::checkProgramCompile(GLuint programID) {
    int success;
    char logs[1024];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 1024, NULL, logs);
        std::cout << "PROGRAM LINKING ERROR:" << "\n" << logs << std::endl;
        glfwTerminate();
        exit(0);
    }
}

void ComputeShader::use() {
    glUseProgram(ID);
}