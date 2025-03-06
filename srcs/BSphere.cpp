#include "../includes/BSphere.hpp"
#include <limits>



Frustum createFrustumFromCamera(Camera &cam, float aspect, float fovY, float zNear, float zFar) {
    const float DEG2RAD = acos(-1.0f) / 180;
    Frustum frustum;
    const float halfVSide = zFar * tanf(fovY * DEG2RAD * 0.5f);
    const float halfHSide = halfVSide * aspect;
    Vector3 frontMultFar = cam.GetFront() * zFar;

    frustum.nearFace = {cam.GetPosition() + (cam.GetFront() * zNear), cam.GetFront()};
    frustum.farFace = {cam.GetPosition() + frontMultFar, cam.GetFront() * -1};
    frustum.rightFace = {cam.GetPosition(), cross(frontMultFar - (cam.GetRight() * halfHSide), cam.GetUp())};
    frustum.leftFace = {cam.GetPosition(), cross(cam.GetUp(), frontMultFar + cam.GetRight()  * halfHSide)};
    frustum.topFace = {cam.GetPosition(), cross(cam.GetRight(), frontMultFar - cam.GetUp() * halfVSide)};
    frustum.bottomFace = {cam.GetPosition(), cross(frontMultFar+ cam.GetUp() * halfVSide, cam.GetRight())};

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
    

	return BSphere((maxAABB + minAABB) * 0.5f, magnitude(minAABB - maxAABB));
}
