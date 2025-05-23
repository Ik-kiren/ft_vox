#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./NewMesh.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"
#include "./Chunk.hpp"
#include "./SkyBox.hpp"
#include "./Sun.hpp"
#include <string>
#include <unordered_map>

class Renderer
{
private:
    Shader *shader;
    Camera *camera;
    Matrix4 model;

	const int static	TEXTURE_COUNT = 9;
    unsigned int	textureIDs[TEXTURE_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    GLint			textureLocation[TEXTURE_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::string		textureName[TEXTURE_COUNT] = {"dirtTexture", "stoneTexture", "sandTexture", "redSandTexture",
		"snowTexture", "iceTexture", "gravelTexture", "oldGrassTexture", "waterTexture"};
public:
    SkyBox *skyBox;
    Sun     *sun;
    const std::string textureArray[TEXTURE_COUNT] = {"./textures/dirt.png", "./textures/stone.png", "./textures/sand.png", "./textures/red_sand.png", "./textures/snow.png",
        "./textures/ice.png", "./textures/gravel.png", "./textures/grass_carried.png", "./textures/water.png"};
    
    const int STRIDE_SIZE = 6;
    std::unordered_map<unsigned int, NewMesh *> meshes;
    
    Renderer();
    ~Renderer();

    void InitRenderer(Shader *shader, Camera *camera);
    void CreateMesh(unsigned int &meshID);
    unsigned int AddVertex(unsigned int &meshID, Vector3 &vecs, int type);
    unsigned int AddVertex(unsigned int &meshID, float x, float y, float z, int type, Vector2 size, int faceType);
    unsigned int AddVertex2(unsigned int &meshID, float x, float y, float z, int type , Vector2 size, int faceType);
    void addIndices(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3);
    void addIndices2(unsigned int &meshID, unsigned int &v1, unsigned int &v2, unsigned int &v3);
    void FinishMesh(unsigned int &meshID);
    void UpdateMesh(unsigned int &meshID);
    void EraseMesh(unsigned int &meshID);
    void CleanMesh(unsigned int &meshID);
    void Render(unsigned int &meshID);
    void Render(std::vector<Chunk *> &chunks);
    void InitTexture();
    void InitSun(Player *player);

    Renderer &operator=(const Renderer &rhs);
};