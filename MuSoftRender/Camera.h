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
    Eigen::Vector3f target;

public:
    Camera(const Eigen::Vector3f& pos, const Eigen::Vector3f& tgt, const Eigen::Vector3f& up,
           float fov, float aspect, float near, float far) :
        Object(),
        up(up), target(tgt), fov(fov), aspect(aspect), nearPlane(near), farPlane(far)
    {
        SetPosition(pos);
    }

    Eigen::Matrix4f GetViewMatrix() const
    {
        Eigen::Vector3f zaxis = (position - target).normalized();
        Eigen::Vector3f xaxis = up.cross(zaxis).normalized();
        Eigen::Vector3f yaxis = zaxis.cross(xaxis);

        Eigen::Matrix4f viewMatrix;
        viewMatrix << xaxis.x(), xaxis.y(), xaxis.z(), -xaxis.dot(position),
            yaxis.x(), yaxis.y(), yaxis.z(), -yaxis.dot(position),
            zaxis.x(), zaxis.y(), zaxis.z(), -zaxis.dot(position),
            0, 0, 0, 1;

        return viewMatrix;
    }

    Eigen::Matrix4f GetProjectionMatrix() const
    {
        float tanHalfFovy = tan(fov / 2);
        float f = 1.0f / tanHalfFovy;
        float nf = 1.0f / (nearPlane - farPlane);

        Eigen::Matrix4f projMatrix = Eigen::Matrix4f::Zero();
        projMatrix(0, 0) = f / aspect;
        projMatrix(1, 1) = f;
        projMatrix(2, 2) = (farPlane + nearPlane) * nf;
        projMatrix(3, 2) = -1;
        projMatrix(2, 3) = 2 * farPlane * nearPlane * nf;

        return projMatrix;
    }

    void SetPosition(const Eigen::Vector3f& pos)
    {
        position = pos;
    }
    void SetTarget(const Eigen::Vector3f& t)
    {
        target = t;
    }
    void LookAt(const Eigen::Vector3f& point)
    {
        target = point;
    }

    void MoveTowards(const Eigen::Vector3f& direction, float distance)
    {
        Eigen::Vector3f newPos = position + direction.normalized() * distance;
        SetPosition(newPos);
    }
};
