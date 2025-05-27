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
    delete skyBox;
    delete sun;
}

void Renderer::InitShadowMap() {
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    debugDepthQuad = Shader("./shaders/debug_quad.vs", "./shaders/debug_quad_depth.fs");
    simpleDepthShader = Shader("./shaders/shadow_mapping_depth.vs", "./shaders/shadow_mapping_depth.fs");
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);
}

void Renderer::InitRenderer(Shader *shader, Camera *camera) {
    this->shader = shader;
    this->camera = camera;
    model = Matrix4(1);
    this->InitTexture();
    this->skyBox = new SkyBox();
    this->InitShadowMap();
}

void Renderer::InitSun(Player *player) {
    this->sun = new Sun(player);
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

void Renderer::Render(std::vector<Chunk *> &chunks, std::unordered_map<Vector3, Chunk *> &visibility) {
    Matrix4 lightProjection, lightView;
        Matrix4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 1000.0f;
        lightProjection = Orthographique(-200, 200, -200, 200, 0.1, 700);
        //lightProjection = Perspective(120.0f, (SHADOW_WIDTH / SHADOW_HEIGHT), 0.1f, 600.0f);
        Vector3 newCamPos = camera->GetPosition();
        newCamPos.y = 130;
        lightView = lookAt(sun->position, camera->GetPosition(), normalized(cross(newCamPos - sun->position, Vector3(0, 0, -1))));
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMatrix4("model", model);
        simpleDepthShader.setMatrix4("view", lightView);
        simpleDepthShader.setMatrix4("projection", lightProjection);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        for (std::unordered_map<Vector3, Chunk *>::iterator it = visibility.begin(); it != visibility.end(); it++) {
            if (it->second->unload || meshes[it->second->meshID]->GetIndicesArray().size() == 0)
                continue;
            simpleDepthShader.setVector3("offset", meshes[it->second->meshID]->GetPosition());
            glBindVertexArray(meshes[it->second->meshID]->VAO);
            glDrawElements(GL_TRIANGLES, meshes[it->second->meshID]->GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
        }
       glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, 1920 / 2, 1200);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
        glViewport(1920 / 2, 0, 1920, 1200);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    skyBox->SkyBoxShader.use();
    skyBox->SkyBoxShader.setMatrix4("view", camera->GetViewMatrix());
    skyBox->SkyBoxShader.setMatrix4("projection", camera->GetProjectionMat());
    glBindVertexArray(skyBox->skyBoxMeshVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->skyBoxID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    sun->UpdateRender(camera);
    glDepthMask(GL_TRUE);

    shader->use();
    shader->setMatrix4("model", model);
    shader->setMatrix4("view", camera->GetViewMatrix());
    shader->setMatrix4("projection", camera->GetProjectionMat());
    shader->setVector3("cameraPos", camera->GetPosition());
    shader->setVector3("lightPos", sun->position);

    shader->setMatrix4("lightView", lightView);
    shader->setMatrix4("lightProjection", lightProjection);

    for (int i = 0; i < TEXTURE_COUNT; i++) {
        textureLocation[i] = shader->GetUniformLocation(textureName[i]);
        glUniform1i(textureLocation[i], i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
    }
    glUniform1i(shader->GetUniformLocation("shadowMap"),9);
    glActiveTexture(GL_TEXTURE0 + 9);
    glBindTexture(GL_TEXTURE_2D, depthMap);

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