#include "../includes/SkyBox.hpp"

SkyBox::SkyBox() {
    glGenTextures(1, &skyBoxID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data;
    for (size_t i = 0; i < 6; i++)
    {
        data = stbi_load(skyBoxTextureArray[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    //mesh
    glGenVertexArrays(1, &this->skyBoxMeshVAO);
    glGenBuffers(1, &this->skyBoxMeshVBO);
    glBindVertexArray(this->skyBoxMeshVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->skyBoxMeshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, this->skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //shader
    SkyBoxShader = Shader("./shaders/SkyBoxVertex.shader", "./shaders/SkyBoxFragment.shader");
}

SkyBox::~SkyBox() {

}
