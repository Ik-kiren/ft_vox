#pragma once
#include "./ScopMaths.hpp"
#include "Scop.hpp"
#include "./Camera.hpp"



struct Plane {
    Vector3 normal = {0.0f, 1.0f, 0.0f}; 
    float distance = 0.0f;

    Plane(Vector3 p1, Vector3 norm) : normal(normalized(norm)), distance(Dot(normal, p1)) {}

    Plane () {}

    float getSignedDistanceToPlane(Vector3 point) const
	{
		return Dot(normal, point) - distance;
	}
};

struct Frustum {
    Plane topFace;
    Plane bottomFace;
    Plane rightFace;
    Plane leftFace;
    Plane nearFace;
    Plane farFace;
};

struct BoundingVolume
{

	virtual bool isOnOrForwardPlane(const Plane& plane) const = 0;

	bool isOnFrustum(const Frustum& camFrustum) const
	{
		return (isOnOrForwardPlane(camFrustum.leftFace) &&
			isOnOrForwardPlane(camFrustum.rightFace) &&
			isOnOrForwardPlane(camFrustum.topFace) &&
			isOnOrForwardPlane(camFrustum.bottomFace) &&
			isOnOrForwardPlane(camFrustum.nearFace) &&
			isOnOrForwardPlane(camFrustum.farFace));
	};
};

struct BSphere : public BoundingVolume {
    Vector3 center = {0.0f, 0.0f, 0.0f};
    float radius = 0.0f;

    BSphere(const Vector3& inCenter, float inRadius)
		: BoundingVolume{}, center{ inCenter }, radius{ inRadius }
	{}

    BSphere() {}

	bool isOnOrForwardPlane(const Plane& plane) const final
	{
		return plane.getSignedDistanceToPlane(center) > -radius;
	}

    bool isOnFrustum(Frustum& camFrustum, Vector3 globalScale, Matrix4 model) const {
        //Get global scale is computed by doing the magnitude of
        //X, Y and Z model matrix's column.
    
        //Get our global center with process it with the global model matrix of our transform
        Vector3 globalCenter =  model[3];
    
        //To wrap correctly our shape, we need the maximum scale scalar.
        const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);
    
        //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
        BSphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

        //Check Firstly the result that have the most chance
        //to faillure to avoid to call all functions.
        return (globalSphere.isOnOrForwardPlane(camFrustum.leftFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.rightFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.farFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.nearFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.topFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.bottomFace));
    };
};

Frustum createFrustumFromCamera(Camera &cam, float aspect, float fovY, float zNear, float zFar);
BSphere generateSphereBV(std::vector<Vector3> vertices);
