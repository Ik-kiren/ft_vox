#include "../includes/Renderer.hpp"
#include "../includes/Scop.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::CreateMesh(unsigned int &meshID) {
    meshID = this->meshes.size();
    NewMesh newMesh;
    this->meshes.push_back(newMesh);
    
}

unsigned int Renderer::AddVertex(unsigned int meshID, Vector3 vec) {
    unsigned int index = this->meshes[meshID].GetVertexArray().size();
    meshes[meshID].AddVertex(vec);
    return index;
}

void Renderer::addIndices(unsigned int meshID, unsigned int v1, unsigned int v2, unsigned int v3) {
    meshes[meshID].AddIndices(v1, v2, v3);
}

void Renderer::FinishMesh(unsigned int meshID) {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, meshID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->meshes[meshID].GetVertexArray().data()) * this->meshes[meshID].GetVertexArray().size(), this->meshes[meshID].GetVertexArray().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->meshes[meshID].GetVertexArray().data()) * this->meshes[meshID].GetVertexArray().size(), this->meshes[meshID].GetVertexArray().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}