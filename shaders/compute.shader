#version 430 core

struct Plane {
    vec3 normal; 
    float distance;
} Plane_t;

struct Frustum {
    Plane topFace;
    Plane bottomFace;
    Plane rightFace;
    Plane leftFace;
    Plane nearFace;
    Plane farFace;
} Frustum_t;

struct BSphere {
    vec3 center;
    float radius;
} BSphere_t;

struct Instance {
    mat4 matrix;
    BSphere bbox;
} Instance_t;

float getSignedDistanceToPlane(vec3 normal, vec3 point, float distance)
{
	return dot(normal, point) - distance;
}

bool isOnOrForwardPlane(Plane plane, BSphere bbox)
{
	return getSignedDistanceToPlane(plane.normal, bbox.center, plane.distance) > -bbox.radius;
}

bool isOnFrustum(Frustum camFrustum, vec3 globalScale, mat4 model, BSphere bbox) {
    //Get global scale is computed by doing the magnitude of
    //X, Y and Z model matrix's column.

    //Get our global center with process it with the global model matrix of our transform
    vec3 globalCenter =  model[3].xyz;

    //To wrap correctly our shape, we need the maximum scale scalar.
    const float maxScale = max(max(globalScale.x, globalScale.y), globalScale.z);

    //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
    BSphere globalSphere;
    globalSphere.center = globalCenter;
    globalSphere.radius = bbox.radius * (maxScale * 0.5f);

    //Check Firstly the result that have the most chance
    //to faillure to avoid to call all functions.
    return (isOnOrForwardPlane(camFrustum.leftFace, globalSphere) &&
        isOnOrForwardPlane(camFrustum.rightFace, globalSphere) &&
        isOnOrForwardPlane(camFrustum.farFace, globalSphere) &&
        isOnOrForwardPlane(camFrustum.nearFace, globalSphere) &&
        isOnOrForwardPlane(camFrustum.topFace, globalSphere) &&
        isOnOrForwardPlane(camFrustum.bottomFace, globalSphere));
};

layout (local_size_x = 512, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding = 5) buffer bufferData {
    Instance instance[512];
    Frustum frustum;
    uint atomicData;
};

layout (binding = 1, offset = 0) uniform atomic_uint indexes;

void main (void) {
    for (uint i = 0; i < 512; i++) {
        if (isOnFrustum(frustum, vec3(length(instance[gl_GlobalInvocationID.x].matrix[0]), length(instance[gl_GlobalInvocationID.x].matrix[1]), length(instance[gl_GlobalInvocationID.x].matrix[2])), instance[gl_GlobalInvocationID.x].matrix, instance[gl_GlobalInvocationID.x].bbox)) {
            atomicData = 15;
        }
    }
}