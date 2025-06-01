#include "../includes/ShadowMap.hpp"
#include "../includes/ScopMaths.hpp"
#include "../includes/Renderer.hpp"
#include "../includes/Constants.hpp"
#include <GL/glew.h>

ShadowMap::ShadowMap() {
    this->debug = false;
    this->Init();
}

ShadowMap::ShadowMap(bool debug) {
    this->debug = debug;
    this->Init();
}

void ShadowMap::Init() {
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

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER ,planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertices), &PlaneVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    debugDepthQuad = Shader("./shaders/debug_quad.vs", "./shaders/debug_quad_depth.fs");
    simpleDepthShader = Shader("./shaders/shadow_mapping_depth.vs", "./shaders/shadow_mapping_depth.fs");
    planeShadowMapShader = Shader("./shaders/shadow_map_plane.vs", "./shaders/shadow_mapping_depth.fs");
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);
}

ShadowMap::~ShadowMap() {

}

void ShadowMap::SetDebug() {
    if (debugCooldown + 1 < glfwGetTime()) {
        this->debug = this->debug ? false : true;
        this->debug = debug;
        debugCooldown = glfwGetTime();
    }
}

void ShadowMap::Render(Renderer *renderer, std::unordered_map<Vector3, Chunk *> &visibility) {
    Matrix4 lightProjection, lightView;
    float near_plane = 0.1f, far_plane = 1000.0f;
    lightProjection = Orthographique(-400, 400, -400, 400, 0.1, 700);
    //lightProjection = Perspective(120.0f, (SHADOW_WIDTH / SHADOW_HEIGHT), 0.1f, 600.0f);
    Vector3 newCamPos = renderer->camera->GetPosition();
    newCamPos.y = 130;
    lightView = lookAt(renderer->sun->position, newCamPos, normalized(cross(newCamPos - renderer->sun->position, Vector3(0, 0, 1))));
    lightSpaceMatrix = lightView * lightProjection;
    // render scene from light's point of view
    simpleDepthShader.use();
    simpleDepthShader.setMatrix4("model", Matrix4(1.0));
    simpleDepthShader.setMatrix4("view", lightView);
    simpleDepthShader.setMatrix4("projection", lightProjection);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    for (std::unordered_map<Vector3, Chunk *>::iterator it = visibility.begin(); it != visibility.end(); it++) {
        if (it->second->update || it->second->unload || renderer->meshes[it->second->meshID]->GetIndicesArray().size() == 0)
            continue;
        simpleDepthShader.setVector3("offset", renderer->meshes[it->second->meshID]->GetPosition());
        glBindVertexArray(renderer->meshes[it->second->meshID]->VAO);
        glDrawElements(GL_TRIANGLES, renderer->meshes[it->second->meshID]->GetIndicesArray().size(), GL_UNSIGNED_INT, 0);
    }
    planeShadowMapShader.use();
    planeShadowMapShader.setMatrix4("view", lightView);
    planeShadowMapShader.setMatrix4("projection", lightProjection);

    for (int i = 0; i < 2; i++) {
        planeShadowMapShader.setMatrix4("model", Matrix4(1.0));
        planeShadowMapShader.setVector3("offset", planDirection[i] * 20 * 16);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   if (debug) {
        glViewport(0, 0, constants::WINDOW_WIDTH / 2, constants::WINDOW_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        glViewport(constants::WINDOW_WIDTH / 2, 0, constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
    } else {
        glViewport(0, 0, constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
    }
}
