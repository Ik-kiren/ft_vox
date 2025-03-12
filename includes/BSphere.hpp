#pragma once
#include "./ScopMaths.hpp"
#include "./Scop.hpp"
#include "./Camera.hpp"



struct Plane {
    float normal[3] = {0.0f, 1.0f, 0.0f}; 
    float distance = 0.0f;
};


struct Frustum {
    Plane topFace;
    Plane bottomFace;
    Plane rightFace;
    Plane leftFace;
    Plane nearFace;
    Plane farFace;
};

struct BSphere {
    float center[3] = {0.0f, 0.0f, 0.0f};
    float radius = 0.0f;
};

struct Instance {
    Matrix4 matrix;
    BSphere bbox;
};

struct ssboObject {
    Frustum frustum;
    GLuint  atomicData[8];
    Instance instances[8];
};

struct Indirect {
    GLuint  count;
    GLuint  instanceCount;
    GLuint  firstIndex;
    GLuint  baseVertex;
    GLuint  baseInstance;
};

Frustum createFrustumFromCamera(Camera &cam, float aspect, float fovY, float zNear, float zFar);
BSphere generateSphereBV(std::vector<Vector3> vertices);
