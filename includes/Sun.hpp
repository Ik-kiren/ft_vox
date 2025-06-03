#include "./Vector3.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "./Player.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"

class Sun
{
 private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int sunID;

        Shader shader;

        Player  *player;

        float timer;
        float lastTimer;

        const int SUNDISTANCE = 100;
        const int ROTATIONSPEED = 40;

        float sunVertices[18] = {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f,  5.0f,
            0.0f,  5.0f,  5.0f,
            0.0f,  5.0f,  5.0f,
            0.0f,  5.0f, 0.0f,
            0.0f, 0.0f, 0.0f
        };

        void UpdatePosition();

 public:
        Vector3 position;
        Matrix4 model;
        Sun(Player *player);
        ~Sun();

        void UpdateRender(Camera *camera);

};
