#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include "../includes/Mesh.hpp"
#include "../includes/Vector3.hpp"
#include "../includes/stb_image.h"

class ParsingException : public std::exception {
 private:
    std::string msg;
 public:
    ParsingException(std::string str) : msg(str) {}
    const char *what() const throw() {
        return msg.c_str();
    }
};

Mesh::Mesh() {
    components = 0;
}

Mesh::Mesh(std::string str) {
    this->MeshParser(str);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    this->InitTexture();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*meshVertexArray.data()) * meshVertexArray.size(), meshVertexArray.data(), GL_STATIC_DRAW);
    int strideSize = 6;
    if (components == 2)
        strideSize = 9;
    else if (components == 3)
        strideSize = 11;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        strideSize * sizeof(float),
        reinterpret_cast<void*>(0));
    if (components >= 2) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(3 * sizeof(float)));

    }
    if (components == 3) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
            3,
            3,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(8 * sizeof(float)));
    } else {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
            3,
            3,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>((strideSize - 3) * sizeof(float)));
        glEnableVertexAttribArray(4);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("./textures/dirt.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

Mesh::Mesh(const Mesh &mesh) {
    vertices = mesh.vertices;
    normalVertices = mesh.normalVertices;
    textureVertices = mesh.textureVertices;
    faces = mesh.faces;
    verticesIndices = mesh.verticesIndices;
    normalIndices = mesh.normalIndices;
    textureIndices = mesh.textureIndices;
    meshVertexArray = mesh.meshVertexArray;
    components = mesh.components;
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

std::vector<GLfloat>    Mesh::GetVertices() {
    return vertices;
}

std::vector<GLfloat>    Mesh::GetMeshVertexArray() {
    return meshVertexArray;
}

int                     Mesh::GetComponents() {
    return components;
}

void Mesh::MeshCheckLineVertice(char *line) {
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789.-") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
}

void Mesh::MeshCheckLineFace(char *line) {
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789/") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
    if (vertices.size() == 0)
        throw ParsingException("parsing error: no vertices");
}

void Mesh::MeshGetVertice(char *line, int lineNbr) {
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    int i = 0;

    while (std::getline(streamLine, buffer, ' ')) {
        vertices.push_back(std::stof(buffer.c_str()));
        i++;
    }
    if (i != 3)
        throw ParsingException("parsing error: vertices line: " + std::to_string(lineNbr) + " " + tmpLine);
}

void Mesh::MeshGetNormalVertice(char *line, int lineNbr) {
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    int i = 0;

    while (std::getline(streamLine, buffer, ' ')) {
        normalVertices.push_back(std::stof(buffer.c_str()));
        i++;
    }
    if (i != 3)
        throw ParsingException("parsing error: normal vertices line: " + std::to_string(lineNbr) + " " + tmpLine);
}

void Mesh::MeshGetTextureVertice(char *line, int lineNbr) {
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    int i = 0;

    while (std::getline(streamLine, buffer, ' ')) {
        textureVertices.push_back(std::stof(buffer.c_str()));
        i++;
    }
    if (i != 2)
        throw ParsingException("parsing error: texture vertices line: " + std::to_string(lineNbr) + " " + tmpLine);
}

void Mesh::MeshGetFace(char *line, int lineNbr) {
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    std::vector<std::string> dividedLine;
    std::vector<std::string> dividedFaces;
    int                      coloIndices = lineNbr % 5;

    Vector3 blackShades[5] = {Vector3(0.0, 0.0, 0.0), Vector3(0.05, 0.05, 0.05), Vector3(0.1, 0.1, 0.1),
        Vector3(0.15, 0.15, 0.15), Vector3(0.2, 0.2, 0.2)};
    while (std::getline(streamLine, buffer, ' ')) {
        dividedLine.push_back(buffer);
        dividedFaces.push_back(buffer);
        faces.push_back(atoi(buffer.c_str()) - 1);
    }

    if (dividedLine.size() == 4) {
        faces.pop_back();
        dividedFaces.pop_back();
        dividedLine.erase(dividedLine.begin() + 1);
        for (size_t i = 0; i < 3; i++) {
            dividedFaces.push_back(dividedLine[i]);
            faces.push_back(atoi(dividedLine[i].c_str()) - 1);
        }
    }
    for (size_t i = 0; i < dividedFaces.size(); i++) {
        std::stringstream tmp(dividedFaces[i]);
        std::vector<std::string> tmpFaces;
        int facesNbrCheck = 0;
        while (std::getline(tmp, buffer, '/')) {
            if (buffer[0] != '\0')
                facesNbrCheck++;
            tmpFaces.push_back(buffer);
        }
        if ((facesNbrCheck > 1 && normalVertices.size() == 0) || (facesNbrCheck > 1 && textureVertices.size() == 0))
            throw ParsingException("error: faces indices reference non existant vertex: " +
                std::to_string(lineNbr) + " " + tmpLine);

        for (size_t j = 0; j < tmpFaces.size(); j++) {
            if (j == 2) {
                meshVertexArray.push_back(textureVertices[(atoi(tmpFaces[j - 1].c_str()) - 1) * 2]);
                meshVertexArray.push_back(textureVertices[(atoi(tmpFaces[j - 1].c_str()) - 1) * 2 + 1]);
            } else if (j == 1 && tmpFaces.size() > 2) {
                meshVertexArray.push_back(normalVertices[(atoi(tmpFaces[j + 1].c_str()) - 1) * 3]);
                meshVertexArray.push_back(normalVertices[(atoi(tmpFaces[j + 1].c_str()) - 1) * 3 + 1]);
                meshVertexArray.push_back(normalVertices[(atoi(tmpFaces[j + 1].c_str()) - 1) * 3 + 2]);
            } else {
                meshVertexArray.push_back(vertices[(atoi(tmpFaces[j].c_str()) - 1) * 3]);
                meshVertexArray.push_back(vertices[(atoi(tmpFaces[j].c_str()) - 1) * 3 + 1]);
                meshVertexArray.push_back(vertices[(atoi(tmpFaces[j].c_str()) - 1) * 3 + 2]);
            }
        }
        meshVertexArray.push_back(blackShades[coloIndices].x);
        meshVertexArray.push_back(blackShades[coloIndices].y);
        meshVertexArray.push_back(blackShades[coloIndices].z);
        if (i > 0 && facesNbrCheck != components)
            throw ParsingException("parsing error: faces numbers: " + std::to_string(lineNbr) + " " + tmpLine);
        components = tmpFaces.size();
    }
}

void Mesh::MeshParser(std::string fileName) {
    std::ifstream file;
    char buffer[500];
    int lineNbr = 0;
    try {
        file.open(fileName);
        if (!file.good())
            throw ParsingException("parsing error: file " + fileName + " does not exist");
        while (file.good()) {
            lineNbr++;
            file.getline(buffer, 500);
            std::string strbuffer(buffer);
            if (strbuffer.size() > 3 && buffer[0] == 'v' && buffer[1] == ' ') {
                MeshCheckLineVertice(buffer + 2);
                MeshGetVertice(buffer + 2, lineNbr);
            }
            if (strbuffer.size() > 4 && buffer[0] == 'v' && buffer[1] == 'n' && buffer[2] == ' ') {
                MeshCheckLineVertice(buffer + 3);
                MeshGetNormalVertice(buffer + 3, lineNbr);
            }
            if (strbuffer.size() > 4 && buffer[0] == 'v' && buffer[1] == 't' && buffer[2] == ' ') {
                MeshCheckLineVertice(buffer + 3);
                MeshGetTextureVertice(buffer + 3, lineNbr);
            }
            if (strbuffer.size() > 2 && buffer[0] == 'f') {
                MeshCheckLineFace(buffer + 2);
                MeshGetFace(buffer + 2, lineNbr);
            }
        }
        if (meshVertexArray.size() == 0)
            throw ParsingException("parsing error: no faces");
        file.close();
    }
    catch(ParsingException &e) {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        exit(0);
    }
}

Mesh &Mesh::operator=(Mesh const &rhs) {
    this->VAO = rhs.VAO;
    std::cout << rhs.VAO << std::endl;
    this->VBO = rhs.VBO;
    this->EBO = rhs.EBO;
    this->texture = rhs.texture;
    this->vertices = rhs.vertices;
    this->normalVertices = rhs.normalVertices;
    this->textureVertices = rhs.textureVertices;
    this->faces = rhs.faces;
    this->verticesIndices = rhs.verticesIndices;
    this->normalIndices = rhs.normalIndices;
    this->textureIndices = rhs.textureIndices;
    this->meshVertexArray = rhs.meshVertexArray;
    this->components = rhs.components;
    return *this;
}

GLuint Mesh::getVAO() {
    return VAO;
}

GLuint Mesh::getVBO() {
    return VBO;
}

GLuint Mesh::getEBO() {
    return EBO;
}

GLuint Mesh::getTexture() {
    return texture;
}

void Mesh::Print() {
    std::cout << "Mesh:" << std::endl;
    std::cout << vertices.size() << std::endl;
    std::cout << VAO << std::endl;
    std::cout << VBO << std::endl;
    std::cout << EBO << std::endl;
    std::cout << texture << std::endl;
    std::cout << normalVertices.size() << std::endl;
    std::cout << textureVertices.size() << std::endl;
    std::cout << faces.size() << std::endl;
    std::cout << verticesIndices.size() << std::endl;
    std::cout << normalIndices.size() << std::endl;
    std::cout << textureIndices.size() << std::endl;
    std::cout << meshVertexArray.size() << std::endl;
    std::cout << components << std::endl;
    std::cout << "End Mesh:" << std::endl;
}