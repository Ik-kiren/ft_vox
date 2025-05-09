#include "../includes/NewMesh.hpp"
#include <iostream>

unsigned long int NewMesh::meshID = 0;

NewMesh::NewMesh() {
    this->textureIndex = 0;
    NewMesh::meshID += 1;
}

NewMesh::~NewMesh() {}

std::vector<int> NewMesh::GetVertexArray() {
    return this->vertexArray;
}

std::vector<unsigned int> NewMesh::GetIndicesArray() {
    return this->indicesArray;
}

void NewMesh::CleanMeshData() {
    this->indicesArray.clear();
    this->vertexArray.clear();
}

void NewMesh::AddVertex(Vector3 &vec) {
    for (int i = 0; i < 3; i++) {
        this->vertexArray.push_back(vec[i]);   
    }
}

void NewMesh::AddVertex(Vector2 vec) {
    for (int i = 0; i < 2; i++) {
        this->vertexArray.push_back(vec[i]);   
    }
}

void NewMesh::AddVertex(float f1, float f2) {
    this->vertexArray.push_back(f1);
    this->vertexArray.push_back(f2);
}

void NewMesh::AddVertex(float x, float y, float z) {
    this->vertexArray.push_back(x);
    this->vertexArray.push_back(y);
    this->vertexArray.push_back(z);
}

void NewMesh::AddInt(int nbr) {
    this->vertexArray.push_back(nbr);
}

void NewMesh::AddIndices(unsigned int v1, unsigned int v2, unsigned int v3) {
    this->indicesArray.push_back(v1);
    this->indicesArray.push_back(v2);
    this->indicesArray.push_back(v3);
}

void    NewMesh::SetPosition(Vector3 vec) {
    position = vec;
}

Vector3 NewMesh::GetPosition() {
    return position;
}