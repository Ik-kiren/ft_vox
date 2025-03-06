#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./Matrix4.hpp"
#include "./Shader.hpp"
#include "./Mesh.hpp"
#include "./Scop.hpp"
#include "./Camera.hpp"
#include "./BSphere.hpp"

class Object {
 private:
    std::vector<GLfloat>      vertices;
    std::vector<GLuint>       indices;
    std::vector<Vector3>      vecVertices;
    Matrix4                   model;
    Matrix4                   projection;
    Shader                    meshShader;
    Mesh                      *mesh;
    BSphere                   sphere;

    Vector3                   position;
    Vector4                   color;

    bool                      activeTexture;
    double                    timer;

    bool                      textureTransition = false;
    double                    timerTextureTransition = 1.0;

 public:
    Object();
    Object(Shader &meshShader, Mesh *mesh);
    Object(Shader &meshShader, Mesh *mesh, Vector3 color);
    Object(Shader &meshShader, Mesh *mesh, Vector4 color);
    Object(const Object &obj);
    ~Object();

    void                    InitTexture();
    std::vector<GLfloat>    getVertices();
    std::vector<GLuint>     getIndices();
    float                   getOffsetZ();
    float                   getOffsetY();
    float                   getOffsetX();
    Vector3                 getOffset();
    Vector3                 GetPosition();
    GLuint                  getVao();
    Matrix4                 *getModel();
    Vector4                 getColor();
    BSphere                 getSphere();
    void                    SetModel(Matrix4 newModel);
    void                    SetColor(Vector4 color);
    void                    bindVao();
    void                    translate(Vector3 vec);
    void                    rotate(Vector3 vec);
    void                    rotate(Vector3 vec, float speed);
    void                    scale(float scale);
    void                    drawMeshInstance(GLFWwindow *window, Camera &camera, std::vector<Object> &instance);
    void                    drawMesh(GLFWwindow *window, Camera &camera);
    void                    drawMesh(GLFWwindow *window, Camera camera, Vector3 lightPos);
    void                    drawMesh(GLFWwindow *window, Camera &camera, Matrix4 newModel);
    void                    drawMesh(GLFWwindow *window, Camera &camera, Matrix4 newModel, Vector4 color);
    void                    TextureTransition();
    Vector3                 GetGlobalScale();
    Object                  *clone();
    void                    print();

    Object &operator=(const Object &rhs);
};
