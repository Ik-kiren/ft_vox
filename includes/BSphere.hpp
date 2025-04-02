#pragma once
#include "./ScopMaths.hpp"
#include "./Scop.hpp"

class Camera;

struct Plane {
    float normal[3] = {0.0f, 1.0f, 0.0f}; 
    float distance = 0.0f;

    float GetDistanceToPlane(Vector3 point, Camera *camera);
};

struct BSphere {
    float center[3] = {0.0f, 0.0f, 0.0f};
    float radius = 0.0f;

    bool IsOnForwardPlane(Plane &plane, Camera *camera ) {
        std::cout << "center " << center[0] << " " << center[1] << " " << center[2] << " " << radius << std::endl;
        return (plane.GetDistanceToPlane(Vector3(center[0], center[1], center[2]), camera) > -radius);
    }
};

struct Frustum {
    Plane topFace;
    Plane bottomFace;
    Plane rightFace;
    Plane leftFace;
    Plane nearFace;
    Plane farFace;

    bool IsInFrustum(BSphere &bsphere, Camera *camera) {
        return (bsphere.IsOnForwardPlane(rightFace, camera));
                // bsphere.IsOnForwardPlane(bottomFace) &&
                // bsphere.IsOnForwardPlane(rightFace) &&
                // bsphere.IsOnForwardPlane(leftFace) &&
                // bsphere.IsOnForwardPlane(nearFace) &&
                // bsphere.IsOnForwardPlane(farFace));
    }

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
Vector3 oneToThree(int idx, int sizeX, int sizeY, int sizeZ);
