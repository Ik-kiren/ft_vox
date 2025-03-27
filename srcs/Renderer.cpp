#include "../includes/Renderer.hpp"
#include "../includes/Scop.hpp"

Renderer::Renderer(Shader &shader, Camera &camera) : shader(shader), camera(camera) {
    this->textureIndex = 0;
    this->InitTexture();

    glGenBuffers(1, &textureSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, textureSSBO);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(textureVertices), &textureVertices, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, textureSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    
    atomicID = 0;
    glGenBuffers(1, &atomicID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, atomicID);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(atomicCounter), &atomicCounter, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, atomicCounter);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

Renderer::~Renderer() {}

void Renderer::CreateMesh(unsigned int &meshID) {
    meshID = this->meshes.size();
    NewMesh newMesh;
    this->meshes.push_back(newMesh);
    model = Matrix4(1);
    
}

unsigned int Renderer::AddVertex(unsigned int meshID, Vector3 vec, float type) {
    unsigned int index = this->meshes[meshID].GetVertexArray().size() / STRIDE_SIZE;
    meshes[meshID].AddVertex(vec);
    meshes[meshID].AddVertex(this->textureVertices[this->textureIndex]);
    meshes[meshID].AddFloat(type);
    this->textureIndex = (this->textureIndex + 1) % 4;
    return index;
}

void Renderer::addIndices(unsigned int meshID, unsigned int v1, unsigned int v2, unsigned int v3) {
    meshes[meshID].AddIndices(v1, v2, v3);
}

void Renderer::FinishMesh(unsigned int &meshID) {
    this->textureIndex = 0;
    glGenVertexArrays(1, &this->meshes[meshID].VAO);
    glGenBuffers(1, &this->meshes[meshID].VBO);
    glGenBuffers(1, &this->meshes[meshID].EBO);
    glBindVertexArray(this->meshes[meshID].VAO);
    

    glBindBuffer(GL_ARRAY_BUFFER, this->meshes[meshID].VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*this->meshes[meshID].GetVertexArray().data()) * this->meshes[meshID].GetVertexArray().size(), this->meshes[meshID].GetVertexArray().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshes[meshID].EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*this->meshes[meshID].GetIndicesArray().data()) * this->meshes[meshID].GetIndicesArray().size(), this->meshes[meshID].GetIndicesArray().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::Render() {
    shader.use();

    shader.setVector4("newColor", Vector3(0.2, 0.5, 0.8));
    shader.setMatrix4("model", model);
    shader.setMatrix4("view", camera.GetViewMatrix());
    shader.setMatrix4("projection", camera.GetProjectionMat());
    shader.setVector3("cameraPos", camera.GetPosition());
    shader.setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    shader.setBool("activeTexture", true);
    shader.setFloat("timerTextureTransition", 1.0f);

    GLint dirtTexture = shader.GetUniformLocation("dirtTexture");
    GLint stoneTexture = shader.GetUniformLocation("stoneTexture");

    glUniform1i(dirtTexture, 0);
    glUniform1i(stoneTexture, 1);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, textureID2);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, textureSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, atomicID);
    for (size_t i = 0; i < meshes.size(); i++) {
        shader.setVector3("offset", meshes[i].GetPosition());
        glBindVertexArray(meshes[i].VAO);
        //glDrawArrays(GL_TRIANGLES, 0, meshes[0].GetVertexArray().size());
        glDrawElements(GL_TRIANGLES, meshes[i].GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Renderer::Render(unsigned int meshID) {
    shader.use();

    shader.setVector4("newColor", Vector3(0.2, 0.5, 0.8));
    shader.setMatrix4("model", model);
    shader.setMatrix4("view", camera.GetViewMatrix());
    shader.setMatrix4("projection", camera.GetProjectionMat());
    shader.setVector3("cameraPos", camera.GetPosition());
    shader.setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    shader.setBool("activeTexture", true);
    shader.setFloat("timerTextureTransition", 0.0f);
    shader.setVector3("offset", meshes[meshID].GetPosition());

    glBindVertexArray(meshes[meshID].VAO);
    //glDrawArrays(GL_TRIANGLES, 0, meshes[0].GetVertexArray().size());
    glDrawElements(GL_TRIANGLES, meshes[meshID].GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::InitTexture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glbindtextures()

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("./textures/dirt.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &textureID2);
    glBindTexture(GL_TEXTURE_2D, textureID2);
    //glbindtextures()

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width2, height2, nrChannels2;
    unsigned char *data2 = stbi_load("./textures/stone.png", &width2, &height2, &nrChannels2, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);
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
    this->textureIndex = rhs.textureIndex;
    return *this;
}