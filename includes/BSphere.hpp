#pragma once
#include "./ScopMaths.hpp"
#include "./Scop.hpp"

class Camera;

struct Plane {
    float normal[3] = {0.0f, 1.0f, 0.0f}; 
    float distance = 0.0f;

    float GetDistanceToPlane(Vector3 point);
};

struct BSphere {
    float center[3] = {0.0f, 0.0f, 0.0f};
    float radius = 0.0f;

    bool IsOnForwardPlane(Plane &plane) {
        return (plane.GetDistanceToPlane(Vector3(center[0], center[1], center[2])) > -radius);
    }
};

struct AABB {
    float center[3] = {0.0f, 0.0f, 0.0f};
    float extents[3] = {8.5f, 8.5f, 8.5f};

    bool IsOnOrForwardPlane(Plane &plane) {
        const float r = extents[0] * std::abs(plane.normal[0]) + extents[1] * std::abs(plane.normal[1]) + extents[2] * std::abs(plane.normal[2]);

        return -r <= plane.GetDistanceToPlane(Vector3(center[0], center[1], center[2]));
    }
};

struct Frustum {
    Plane topFace;
    Plane bottomFace;
    Plane rightFace;
    Plane leftFace;
    Plane nearFace;
    Plane farFace;

    bool IsInFrustum(BSphere &bsphere) {
        return (bsphere.IsOnForwardPlane(topFace) &&
                bsphere.IsOnForwardPlane(bottomFace) &&
                bsphere.IsOnForwardPlane(rightFace) &&
                bsphere.IsOnForwardPlane(leftFace) &&
                bsphere.IsOnForwardPlane(nearFace) &&
                bsphere.IsOnForwardPlane(farFace));
    }

    bool IsInFrustum(AABB &aabb) {
        return (aabb.IsOnOrForwardPlane(topFace) &&
                aabb.IsOnOrForwardPlane(bottomFace) &&
                aabb.IsOnOrForwardPlane(rightFace) &&
                aabb.IsOnOrForwardPlane(leftFace) &&
                aabb.IsOnOrForwardPlane(nearFace) &&
                aabb.IsOnOrForwardPlane(farFace));
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
int threeToOne(int x , int y, int z);
