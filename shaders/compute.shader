#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 16) in;

layout (std430, binding = 0) buffer bufferData {
    //matrix object
    //frustum matrix ?
};

layout (binding = 1, offset = 0) uniform atomic_uint indexes;

void main (void) {

}