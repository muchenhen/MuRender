#pragma once
#include "Object.h"
class Camera : public Object
{
private:
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;
    Eigen::Vector3f up;
    // 纠结一下用什么摄像机
    // Eigen::Vector3f target;

public:
    Camera(const Eigen::Vector3f& pos, const Eigen::Vector3f& tgt, const Eigen::Vector3f& up,
           float fov, float aspect, float near, float far) :
        Object(),
        up(up), fov(fov), aspect(aspect), nearPlane(near), farPlane(far)
    {
        SetPosition(pos);
    }
};
