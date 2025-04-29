#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./NewMesh.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"
#include "./Chunk.hpp"
#include <string>
#include <mutex>
#include <unordered_map>

class Renderer
{
private:
    Shader *shader;
    Camera *camera;
    Matrix4 model;

    unsigned int atomicID;
    unsigned int atomicCounter;

    unsigned int textureID;
    unsigned int textureID2;
    unsigned int textureSSBO;

    std::vector<float> normalVertices;
    std::mutex renderMutex;
	const int static	nbr = 9;
    unsigned int	textureIDs[nbr] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    GLint			textureLocation[nbr] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::string		textureName[nbr] = {"dirtTexture", "stoneTexture", "sandTexture", "redSandTexture",
		"snowTexture", "iceTexture", "gravelTexture", "oldGrassTexture", "waterTexture"};
public:
    const std::string textureArray[nbr] = {"./textures/dirt.png", "./textures/stone.png", "./textures/sand.png", "./textures/red_sand.png", "./textures/snow.png",
        "./textures/ice.png", "./textures/gravel.png", "./textures/grass_carried.png", "./textures/water.png"};
    
    const int STRIDE_SIZE = 6;
    std::unordered_map<unsigned int, NewMesh *> meshes;
    
    Renderer();
    ~Renderer();

    void InitRenderer(Shader *shader, Camera *camera);
    void CreateMesh(unsigned int &meshID);
    unsigned int AddVertex(unsigned int &meshID, Vector3 &vecs, int type);
    unsigned int AddVertex(unsigned int &meshID, float x, float y, float z, int type, Vector2 size, int faceType);
    void addIndices(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3);
    void FinishMesh(unsigned int &meshID);
    void EraseMesh(unsigned int &meshID);
    void Render();
    void Render(unsigned int &meshID);
    void Render(std::vector<Chunk *> &chunks);
    void InitTexture();

    Renderer &operator=(const Renderer &rhs);
};