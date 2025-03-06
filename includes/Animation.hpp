#pragma once
#include <vector>
#include "../includes/Camera.hpp"
#include "../includes/Object.hpp"
#include "../includes/MatrixStack.hpp"

class Animation {
 private:
    double      animationTimer;
    double      stepTime;
    double      stepTimer;
    int         step;
    int         maxStepNbr;
    double      nowTime;
    double      lastTime;
    double      deltaTime;

    Object      *cube;
    GLFWwindow  *window;
    Camera      *camera;

    enum PARTS {HEAD, BODY, RIGHTARM, RIGHTFOREARM, LEFTARM, LEFTFOREARM, RIGHTTHIGH, RIGHTLOWERLEG, LEFTTHIGH, LEFTLOWERLEG};

    Vector3 initPoses[10];

    Vector3 partsScale[10];

    std::vector<Vector3> headPosition;
    std::vector<Vector4> headRotation;
    std::vector<Vector3> bodyPosition;
    std::vector<Vector3> rightArmPosition;
    std::vector<Vector4> rightArmRotation;
    std::vector<Vector3> rightForearmPosition;
    std::vector<Vector4> rightForearmRotation;
    std::vector<Vector3> leftArmPosition;
    std::vector<Vector4> leftArmRotation;
    std::vector<Vector3> leftForearmPosition;
    std::vector<Vector4> leftForearmRotation;
    std::vector<Vector3> rightThighPosition;
    std::vector<Vector4> rightThighRotation;
    std::vector<Vector3> rightLowerLegPosition;
    std::vector<Vector4> rightLowerLegRotation;
    std::vector<Vector3> leftThighPosition;
    std::vector<Vector4> leftThighRotation;
    std::vector<Vector3> leftLowerLegPosition;
    std::vector<Vector4> leftLowerLegRotation;

 public:
    Animation();
    ~Animation();

    int GetStep();

    void AddHeadPosition(Vector3 pos);
    void AddHeadRotation(Vector4 radians);
    void AddBodyPosition(Vector3 pos);
    void AddRightArmPosition(Vector3 pos);
    void AddRightArmRotation(Vector4 radians);
    void AddLeftArmPosition(Vector3 pos);
    void AddLeftArmRotation(Vector4 radians);
    void AddRightForearmPosition(Vector3 pos);
    void AddRightForearmRotation(Vector4 radians);
    void AddLeftForearmPosition(Vector3 pos);
    void AddLeftForearmRotation(Vector4 radians);
    void AddRightThighPosition(Vector3 pos);
    void AddRightThighRotation(Vector4 radians);
    void AddRightLowerLegPosition(Vector3 pos);
    void AddRightLowerLegRotation(Vector4 radians);
    void AddLeftThighPosition(Vector3 pos);
    void AddLeftThighRotation(Vector4 radians);
    void AddLeftLowerLegPosition(Vector3 pos);
    void AddLeftLowerLegRotation(Vector4 radians);
    void AddArmsPosition(Vector3 pos);
    void AddArmsRotation(Vector4 radians);
    void AddThighsPosition(Vector3 pos);
    void AddThighsRotation(Vector4 radians);
    void AddLowerLegsPosition(Vector3 pos);
    void AddLowerLegsRotation(Vector4 radians);
    
    void HeadAnimation(MatrixStack &mat);
    void BodyAnimation(MatrixStack &mat);
    void ArmsAnimation(MatrixStack &mat);
    void LegsAnimation(MatrixStack &mat);

    void Jump(GLFWwindow *window, Camera &camera, Object &body, Object &head, Object &arm);
    void Play(GLFWwindow *window, Camera &camera, Object &cube);
};
