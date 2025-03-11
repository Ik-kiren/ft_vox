#include "../includes/BSphere.hpp"
#include <limits>

Plane CreatePlane(Vector3 p1, Vector3 norm) {
    Plane tmp;
    Vector3 normale = normalized(norm);
    tmp.normal[0] = normale.x;
    tmp.normal[1] = normale.y;
    tmp.normal[2] = normale.z;
    tmp.distance = Dot(normale, p1);
    return tmp;
}

BSphere CreateBSphere(const Vector3& inCenter, float inRadius) {
    BSphere tmp;
    tmp.center[0] = inCenter.x;
    tmp.center[1] = inCenter.y;
    tmp.center[2] = inCenter.z;
    tmp.radius = inRadius;
    return tmp;
}

Frustum createFrustumFromCamera(Camera &cam, float aspect, float fovY, float zNear, float zFar) {
    const float DEG2RAD = acos(-1.0f) / 180;
    Frustum frustum;
    const float halfVSide = zFar * tanf(fovY * DEG2RAD * 0.5f);
    const float halfHSide = halfVSide * aspect;
    Vector3 frontMultFar = cam.GetFront() * zFar;
    
    
    frustum.nearFace = CreatePlane(cam.GetPosition() + (cam.GetFront() * zNear), cam.GetFront());
    frustum.farFace = CreatePlane(cam.GetPosition() + frontMultFar, cam.GetFront() * -1);
    frustum.rightFace = CreatePlane(cam.GetPosition(), cross(frontMultFar - (cam.GetRight() * halfHSide), cam.GetUp()));
    frustum.leftFace = CreatePlane(cam.GetPosition(), cross(cam.GetUp(), frontMultFar + cam.GetRight()  * halfHSide));
    frustum.topFace = CreatePlane(cam.GetPosition(), cross(cam.GetRight(), frontMultFar - cam.GetUp() * halfVSide));
    frustum.bottomFace = CreatePlane(cam.GetPosition(), cross(frontMultFar+ cam.GetUp() * halfVSide, cam.GetRight()));

    return frustum;
}

BSphere generateSphereBV(std::vector<Vector3> vertices)
{
	Vector3 minAABB = Vector3(std::numeric_limits<float>::max());
	Vector3 maxAABB = Vector3(std::numeric_limits<float>::min());

    for (size_t i = 0; i < vertices.size(); i++) {
        minAABB.x = std::min(minAABB.x, vertices[i].x);
		minAABB.y = std::min(minAABB.y, vertices[i].y);
		minAABB.z = std::min(minAABB.z, vertices[i].z);

		maxAABB.x = std::max(maxAABB.x, vertices[i].x);
		maxAABB.y = std::max(maxAABB.y, vertices[i].y);
		maxAABB.z = std::max(maxAABB.z, vertices[i].z);
    }
    


	return CreateBSphere((maxAABB + minAABB) * 0.5f, magnitude(minAABB - maxAABB));
}
