#include "../includes/Renderer.hpp"
#include "../includes/Scop.hpp"
#include <numbers>
#include <ctime>

Renderer::Renderer() {
}

Renderer::~Renderer() {
    for (size_t i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
    delete skyBox;
}

void Renderer::InitRenderer(Shader *shader, Camera *camera) {
    this->shader = shader;
    this->camera = camera;
    model = Matrix4(1);
    this->InitTexture();
    this->skyBox = new SkyBox();
}

void Renderer::CreateMesh(unsigned int &meshID) {
    NewMesh *newMesh = new NewMesh;
    meshID = NewMesh::meshID - 1;
    this->meshes.insert({NewMesh::meshID - 1, newMesh});
}

unsigned int Renderer::AddVertex(unsigned int &meshID, Vector3 &vec, int type) {
    unsigned int index = this->meshes[meshID]->GetVertexArray().size() / STRIDE_SIZE;
    meshes[meshID]->AddVertex(vec);
    meshes[meshID]->AddVertex(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex]);
    meshes[meshID]->AddInt(type);
    meshes[meshID]->textureIndex = (meshes[meshID]->textureIndex + 1) % 4;
    return index;
}

unsigned int Renderer::AddVertex(unsigned int &meshID, float x, float y, float z, int type , Vector2 size, int faceType) {
    int tmp = 0;
    unsigned int index = this->meshes[meshID]->GetVertexArray().size() / 2;
    tmp = (int)x;
    tmp = (tmp << 5) + (int)y;
    tmp = (tmp << 5) + (int)z;
    tmp = (tmp << 5) + (int)(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].x * size.x);
    tmp = (tmp << 5) + (int)(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].y * size.y);
    tmp = (tmp << 4) + type;
    
    meshes[meshID]->AddInt(tmp);
    meshes[meshID]->AddInt(faceType);

    meshes[meshID]->textureIndex = (meshes[meshID]->textureIndex + 1) % 4; // look to change that
    return index;
}

unsigned int Renderer::AddVertex2(unsigned int &meshID, float x, float y, float z, int type , Vector2 size, int faceType) {
    int tmp = 0;
    unsigned int index = this->meshes[meshID]->GetVertexArray2().size() / 2;
    tmp = (int)x;
    tmp = (tmp << 5) + (int)y;
    tmp = (tmp << 5) + (int)z;
    tmp = (tmp << 5) + (int)(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].x * size.x);
    tmp = (tmp << 5) + (int)(meshes[meshID]->textureVertices[meshes[meshID]->textureIndex].y * size.y);
    tmp = (tmp << 4) + type;
    
    meshes[meshID]->AddInt2(tmp);
    meshes[meshID]->AddInt2(faceType);

    meshes[meshID]->textureIndex = (meshes[meshID]->textureIndex + 1) % 4; // look to change that
    return index;
}

void Renderer::addIndices(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3) {
    meshes[meshID]->AddIndices(v1, v2, v3);
}

void Renderer::addIndices2(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3) {
    meshes[meshID]->AddIndices2(v1, v2, v3);
}

void Renderer::FinishMesh(unsigned int &meshID) {
    if (!this->meshes[meshID]->update) {
        glGenVertexArrays(1, &this->meshes[meshID]->VAO);
        glGenBuffers(1, &this->meshes[meshID]->VBO);
        glGenBuffers(1, &this->meshes[meshID]->EBO);
        glBindVertexArray(this->meshes[meshID]->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->meshes[meshID]->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetVertexArray().data()) * this->meshes[meshID]->GetVertexArray().size(), this->meshes[meshID]->GetVertexArray().data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshes[meshID]->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetIndicesArray().data()) * this->meshes[meshID]->GetIndicesArray().size(), this->meshes[meshID]->GetIndicesArray().data(), GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 1, GL_INT, sizeof(int) * 2, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(1, 1, GL_INT, sizeof(int) * 2, (void*)(sizeof(int) * 1));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &this->meshes[meshID]->VAO2);
        glGenBuffers(1, &this->meshes[meshID]->VBO2);
        glGenBuffers(1, &this->meshes[meshID]->EBO2);
        glBindVertexArray(this->meshes[meshID]->VAO2);

        glBindBuffer(GL_ARRAY_BUFFER, this->meshes[meshID]->VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetVertexArray2().data()) * this->meshes[meshID]->GetVertexArray2().size(), this->meshes[meshID]->GetVertexArray2().data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshes[meshID]->EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetIndicesArray2().data()) * this->meshes[meshID]->GetIndicesArray2().size(), this->meshes[meshID]->GetIndicesArray2().data(), GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 1, GL_INT, sizeof(int) * 2, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(1, 1, GL_INT, sizeof(int) * 2, (void*)(sizeof(int) * 1));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    } else {
        glBindVertexArray(this->meshes[meshID]->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->meshes[meshID]->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetVertexArray().data()) * this->meshes[meshID]->GetVertexArray().size(), this->meshes[meshID]->GetVertexArray().data(), GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshes[meshID]->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetIndicesArray().data()) * this->meshes[meshID]->GetIndicesArray().size(), this->meshes[meshID]->GetIndicesArray().data(), GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 1, GL_INT, sizeof(int) * 2, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(1, 1, GL_INT, sizeof(int) * 2, (void*)(sizeof(int) * 1));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(this->meshes[meshID]->VAO2);

        glBindBuffer(GL_ARRAY_BUFFER, this->meshes[meshID]->VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetVertexArray2().data()) * this->meshes[meshID]->GetVertexArray2().size(), this->meshes[meshID]->GetVertexArray2().data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshes[meshID]->EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*this->meshes[meshID]->GetIndicesArray2().data()) * this->meshes[meshID]->GetIndicesArray2().size(), this->meshes[meshID]->GetIndicesArray2().data(), GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 1, GL_INT, sizeof(int) * 2, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(1, 1, GL_INT, sizeof(int) * 2, (void*)(sizeof(int) * 1));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        this->meshes[meshID]->update = false;
    }
}

void Renderer::UpdateMesh(unsigned int &meshID) {
    glNamedBufferData(this->meshes[meshID]->VBO, sizeof(*this->meshes[meshID]->GetVertexArray().data()) * this->meshes[meshID]->GetVertexArray().size(), this->meshes[meshID]->GetVertexArray().data(), GL_DYNAMIC_DRAW);
    glNamedBufferData(this->meshes[meshID]->EBO, sizeof(*this->meshes[meshID]->GetIndicesArray().data()) * this->meshes[meshID]->GetIndicesArray().size(), this->meshes[meshID]->GetIndicesArray().data(), GL_DYNAMIC_DRAW);
}

void Renderer::EraseMesh(unsigned int &meshID) {
    delete meshes[meshID];
    meshes.erase(meshID);
}

void Renderer::CleanMesh(unsigned int &meshID) {
    if (meshes.find(meshID) != meshes.end()) {
        meshes[meshID]->CleanMeshData();
         meshes[meshID]->update = true;
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
    glDrawElements(GL_TRIANGLES, meshes[meshID]->GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::Render(std::vector<Chunk *> &chunks) {
    glDepthMask(GL_FALSE);
    skyBox->SkyBoxShader.use();
    skyBox->SkyBoxShader.setMatrix4("view", camera->GetViewMatrix());
    skyBox->SkyBoxShader.setMatrix4("projection", camera->GetProjectionMat());
    glBindVertexArray(skyBox->skyBoxMeshVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->skyBoxID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    shader->use();
    shader->setVector4("newColor", Vector3(0.2, 0.5, 0.8));
    shader->setMatrix4("model", model);
    shader->setMatrix4("view", camera->GetViewMatrix());
    shader->setMatrix4("projection", camera->GetProjectionMat());
    shader->setVector3("cameraPos", camera->GetPosition());
    shader->setVector3("lightPos", camera->GetPosition());
    shader->setFloat("timeValue", std::fmod(glfwGetTime() / 10, 2 * std::numbers::pi));
    shader->setBool("activeTexture", true);
    shader->setFloat("timerTextureTransition", 1.0f);


    for (int i = 0; i < TEXTURE_COUNT; i++) {
        textureLocation[i] = shader->GetUniformLocation(textureName[i]);
        glUniform1i(textureLocation[i], i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
    }


    for (size_t i = 0; i < chunks.size(); i++) {
        if (chunks[i]->unload || meshes[chunks[i]->meshID]->GetIndicesArray().size() == 0)
            continue;
        shader->setVector3("offset", meshes[chunks[i]->meshID]->GetPosition());
        glBindVertexArray(meshes[chunks[i]->meshID]->VAO);
        glDrawElements(GL_TRIANGLES, meshes[chunks[i]->meshID]->GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
    }

    glDepthMask(GL_FALSE);
    for (size_t i = 0; i < chunks.size(); i++) {
        if (chunks[i]->unload || meshes[chunks[i]->meshID]->GetIndicesArray2().size() == 0)
            continue;
        shader->setVector3("offset", meshes[chunks[i]->meshID]->GetPosition());
        glBindVertexArray(meshes[chunks[i]->meshID]->VAO2);
        glDrawElements(GL_TRIANGLES, meshes[chunks[i]->meshID]->GetIndicesArray2().size(), GL_UNSIGNED_INT, 0);
    }
    glDepthMask(GL_TRUE);
}

void Renderer::InitTexture() {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
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
    return *this;
}