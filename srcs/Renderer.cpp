#include "../includes/Renderer.hpp"
#include "../includes/Scop.hpp"
#include <sys/time.h>

Renderer::Renderer() {
}

Renderer::~Renderer() {
    for (size_t i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
}

void Renderer::InitRenderer(Shader *shader, Camera *camera) {
    this->shader = shader;
    this->camera = camera;
    model = Matrix4(1);
    this->InitTexture();
}

void Renderer::CreateMesh(unsigned int &meshID) {
    NewMesh *newMesh = new NewMesh;
    //renderMutex.lock();
    meshID = NewMesh::meshID - 1;
    this->meshes.insert({NewMesh::meshID - 1, newMesh});
    //renderMutex.unlock();
}

unsigned int Renderer::AddVertex(unsigned int &meshID, Vector3 &vec, int type) {
    unsigned int index = this->meshes[meshID]->GetVertexArray().size() / STRIDE_SIZE;
    meshes[meshID]->AddVertex(vec);
    meshes[meshID]->AddVertex(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex]);
    meshes[meshID]->AddInt(type);
    meshes[meshID]->textureIndex = (meshes[meshID]->textureIndex + 1) % 4;
    return index;
}

unsigned int Renderer::AddVertex(unsigned int &meshID, float x, float y, float z, int type , Vector2 size) {
    int tmp = 0;
    unsigned int index = this->meshes[meshID]->GetVertexArray().size();
    tmp = (int)x;
    tmp = (tmp << 5) + (int)y;
    tmp = (tmp << 5) + (int)z;
    tmp = (tmp << 5) + (int)(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].x * size.x);
    tmp = (tmp << 5) + (int)(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].y * size.y);
    tmp = (tmp << 4) + type;
    //std::cout << x <<  " " << y << " " << z << " " << meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].x * size.x << " " << meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].y * size.y << " " << type << std::endl;
    //std::cout << ((tmp >> 24) & 31) <<  " " << ((tmp >> 19) & 31) <<  " " << ((tmp >> 14) & 31) <<  " " <<  ((tmp >> 9) & 31) <<  " " << ((tmp >> 4) & 31) <<  " " << (tmp & 15) <<  " " << std::endl;
    
    meshes[meshID]->AddInt(tmp);
    /*meshes[meshID]->AddVertex(x, y, z);
    meshes[meshID]->AddVertex(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex]);
    meshes[meshID]->AddFloat(type);*/
    /*meshes[meshID]->vertexArray.insert(meshes[meshID]->vertexArray.end(), {
        x,
        y,
        z,
        meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].x * size.x,
        meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].y * size.y,
        type
    });*/
    meshes[meshID]->textureIndex = (meshes[meshID]->textureIndex + 1) % 4; // look to change that
    return index;
}

void Renderer::addIndices(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3) {
    meshes[meshID]->AddIndices(v1, v2, v3);
}

void Renderer::FinishMesh(unsigned int &meshID) {

    glGenVertexArrays(1, &this->meshes[meshID]->VAO);
    glGenBuffers(1, &this->meshes[meshID]->VBO);
    glGenBuffers(1, &this->meshes[meshID]->EBO);
    glBindVertexArray(this->meshes[meshID]->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->meshes[meshID]->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetVertexArray().data()) * this->meshes[meshID]->GetVertexArray().size(), this->meshes[meshID]->GetVertexArray().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshes[meshID]->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetIndicesArray().data()) * this->meshes[meshID]->GetIndicesArray().size(), this->meshes[meshID]->GetIndicesArray().data(), GL_STATIC_DRAW);

    glVertexAttribIPointer(0, 1, GL_INT, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::EraseMesh(unsigned int &meshID) {
    delete meshes[meshID];
    meshes.erase(meshID);
}

void Renderer::Render() {
    shader->use();

    shader->setVector4("newColor", Vector3(0.2, 0.5, 0.8));
    shader->setMatrix4("model", model);
    shader->setMatrix4("view", camera->GetViewMatrix());
    shader->setMatrix4("projection", camera->GetProjectionMat());
    shader->setVector3("cameraPos", camera->GetPosition());
    shader->setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    shader->setBool("activeTexture", true);
    shader->setFloat("timerTextureTransition", 1.0f);

    GLint dirtTexture = shader->GetUniformLocation("dirtTexture");
    GLint stoneTexture = shader->GetUniformLocation("stoneTexture");

    glUniform1i(dirtTexture, 0);
    glUniform1i(stoneTexture, 1);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, textureID2);

    for (size_t i = 0; i < meshes.size(); i++) {
        shader->setVector3("offset", meshes[i]->GetPosition());
        glBindVertexArray(meshes[i]->VAO);
        //glDrawArrays(GL_TRIANGLES, 0, meshes[0].GetVertexArray().size());
        glDrawElements(GL_TRIANGLES, meshes[i]->GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Renderer::Render(unsigned int &meshID) {
    shader->use();

    shader->setVector4("newColor", Vector3(0.2, 0.5, 0.8));
    shader->setMatrix4("model", model);
    shader->setMatrix4("view", camera->GetViewMatrix());
    shader->setMatrix4("projection", camera->GetProjectionMat());
    shader->setVector3("cameraPos", camera->GetPosition());
    shader->setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    shader->setBool("activeTexture", true);
    shader->setFloat("timerTextureTransition", 0.0f);
    shader->setVector3("offset", meshes[meshID]->GetPosition());

    glBindVertexArray(meshes[meshID]->VAO);
    //glDrawArrays(GL_TRIANGLES, 0, meshes[0].GetVertexArray().size());
    glDrawElements(GL_TRIANGLES, meshes[meshID]->GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::Render(std::vector<Chunk *> &chunks) {
    shader->use();

    shader->setVector4("newColor", Vector3(0.2, 0.5, 0.8));
    shader->setMatrix4("model", model);
    shader->setMatrix4("view", camera->GetViewMatrix());
    shader->setMatrix4("projection", camera->GetProjectionMat());
    shader->setVector3("cameraPos", camera->GetPosition());
    shader->setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    shader->setBool("activeTexture", true);
    shader->setFloat("timerTextureTransition", 1.0f);


    for (int i = 0; i < nbr; i++) {
        textureLocation[i] = shader->GetUniformLocation(textureName[i]);
        glUniform1i(textureLocation[i], i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
    }

    for (size_t i = 0; i < chunks.size(); i++) {
        if (chunks[i]->unload)
            continue;
        shader->setVector3("offset", meshes[chunks[i]->meshID]->GetPosition());
        glBindVertexArray(meshes[chunks[i]->meshID]->VAO);
        //glDrawArrays(GL_TRIANGLES, 0, meshes[chunks[i]->meshID]->GetIndicesArray().size());
        glDrawElements(GL_TRIANGLES, meshes[chunks[i]->meshID]->GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
    }
}

void Renderer::InitTexture() {
    for (int i = 0; i < nbr; i++) {
        glGenTextures(1, &textureIDs[i]);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(textureArray[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
}

Renderer &Renderer::operator=(const Renderer &rhs) {
    this->shader = rhs.shader;
    this->camera = rhs.camera;
    this->model = rhs.model;
    this->atomicID = rhs.atomicID;
    this->atomicCounter = rhs.atomicCounter;
    this->textureID = rhs.textureID;
    this->textureSSBO = rhs.textureSSBO;
    this->normalVertices = rhs.normalVertices;
    return *this;
}