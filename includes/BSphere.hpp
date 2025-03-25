#pragma once
#include "./ScopMaths.hpp"
#include "./Scop.hpp"



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
    float matrix[4][4];
    BSphere bbox;
};

struct ssboObject {
    Frustum frustum;
    GLuint  atomicData[8];
    Instance instances[27000];
};

struct Indirect {
    GLuint  count = 396;
    GLuint  instanceCount = 0;
    GLuint  first = 0;
    GLuint  baseInstance = 0;
};

BSphere generateSphereBV(std::vector<Vector3> vertices);
Plane CreatePlane(Vector3 p1, Vector3 norm);
