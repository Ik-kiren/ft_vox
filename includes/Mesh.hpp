#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class Mesh {
 private:
    std::vector<GLfloat> vertices;

    GLuint                    VAO;
    GLuint                    VBO;
    GLuint                    EBO;
    GLuint                    texture;

    std::vector<GLfloat> normalVertices;
    std::vector<GLfloat> textureVertices;

    std::vector<GLuint> faces;

    std::vector<GLuint> verticesIndices;
    std::vector<GLuint> normalIndices;
    std::vector<GLuint> textureIndices;

    std::vector<GLfloat> meshVertexArray;

    // 0: v, 1 : v/vn, 2: v/vn/vt
    int components;

 public:
    Mesh();
    Mesh(std::string str);
    Mesh(const Mesh &mesh);
    ~Mesh();

    void InitTexture();
    void MeshCheckLineVertice(char *line);
    void MeshCheckLineFace(char *line);

    std::vector<GLfloat>    GetMeshVertexArray();
    std::vector<GLfloat>    GetVertices();
    int                     GetComponents();

    GLuint getVAO();
    GLuint getVBO();
    GLuint getEBO();
    GLuint getTexture();

    void MeshGetVertice(char *line, int lineNbr);
    void MeshGetNormalVertice(char *line, int lineNbr);
    void MeshGetTextureVertice(char *line, int lineNbr);
    void MeshGetFace(char *line, int lineNbr);
    void MeshParser(std::string fileName);
    void Print();

    Mesh &operator=(Mesh const &rhs);
};
