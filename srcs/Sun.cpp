#include "../includes/Sun.hpp"
#include <cmath>

Sun::Sun(Player *player) : player(player){
    model = Matrix4(1);
    timer = 0;
    lastTimer = 0;
    glGenTextures(1, &sunID);
    glBindTexture(GL_TEXTURE_2D, sunID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data;
    data = stbi_load("./textures/sun.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, this->sunVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->shader = Shader("./shaders/lightVS.shader", "./shaders/lightFS.shader");

   UpdatePosition();
}

Sun::~Sun() {

}


void Sun::UpdatePosition() {
    float revolutionTimer = std::fmod(glfwGetTime() / ROTATIONSPEED, 2 * M_PI);
    position = Vector3(SUNDISTANCE * cos(revolutionTimer) + player->getGlobalPos().x, SUNDISTANCE * sin(revolutionTimer) + 110, player->getGlobalPos().z);
    model[3] = position;
    timer = glfwGetTime() - lastTimer;
    lastTimer = glfwGetTime();
    model = Rotate(model, ((2*M_PI * (60 /((2*M_PI) * ROTATIONSPEED))) / 60) / (1/timer), Vector3(0, 0, 1));
}

void Sun::UpdateRender(Camera *camera) {
    glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
    UpdatePosition();

    shader.use();

    shader.setMatrix4("model", model);
    shader.setMatrix4("view", camera->GetViewMatrix());
    shader.setMatrix4("projection", camera->GetProjectionMat());

    glBindVertexArray(this->VAO);
    glBindTexture(GL_TEXTURE_2D, this->sunID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
