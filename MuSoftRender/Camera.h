#pragma once
#include "Logger.h"
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
public:
    Camera(const Eigen::Vector3f& pos, const Eigen::Vector3f& tgt, const Eigen::Vector3f& upVector,
           float fieldOfView, float aspectRatio, float nearValue, float farValue) :
        Object(),
        target(tgt),
        up(upVector),
        fov(fieldOfView),
        aspect(aspectRatio),
        nearPlane(nearValue),
        farPlane(farValue)
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
        float fovRadians = fov * (3.14 / 180.0f);
        float tanHalfFovy = tan(fovRadians / 2);
        float zRange = farPlane - nearPlane;

        Eigen::Matrix4f projMatrix = Eigen::Matrix4f::Zero();
        projMatrix(0, 0) = 1.0f / (aspect * tanHalfFovy);
        projMatrix(1, 1) = 1.0f / tanHalfFovy;
        projMatrix(2, 2) = -(farPlane + nearPlane) / zRange;
        projMatrix(2, 3) = -2.0f * farPlane * nearPlane / zRange;
        projMatrix(3, 2) = -1.0f;

        // °²È«¼ì²é
        if (projMatrix.array().isNaN().any())
        {
            LOG_ERROR("Warning: Projection matrix contains NaN values!");
        }

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
