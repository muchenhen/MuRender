#pragma once
#include "Functions.h"
#include "Logger.h"
#include "Object.h"

class Camera : public Object
{
private:
    float Fov;
    float Aspect;
    float NearPlane;
    float FarPlane;
    Eigen::Vector3f Up;
    Eigen::Vector3f Target;

public:
    Camera(const Eigen::Vector3f& InPosition, const Eigen::Vector3f& InTarget, const Eigen::Vector3f& InUpVector,
           float InFieldOfView, float InAspectRatio, float InNearValue, float InFarValue) :
        Object(),
        Target(InTarget),
        Up(InUpVector),
        Fov(InFieldOfView),
        Aspect(InAspectRatio),
        NearPlane(InNearValue),
        FarPlane(InFarValue)
    {
        Camera::SetPosition(InPosition);
    }

    Eigen::Matrix4f GetViewMatrix() const
    {
        Eigen::Vector3f AxisZ = (Position - Target).normalized();
        Eigen::Vector3f AxisX = Up.cross(AxisZ).normalized();
        Eigen::Vector3f AxisY = AxisZ.cross(AxisX);

        Eigen::Matrix4f ViewMatrix;
        ViewMatrix << AxisX.x(), AxisX.y(), AxisX.z(), -AxisX.dot(Position),
            AxisY.x(), AxisY.y(), AxisY.z(), -AxisY.dot(Position),
            AxisZ.x(), AxisZ.y(), AxisZ.z(), -AxisZ.dot(Position),
            0, 0, 0, 1;

        return ViewMatrix;
    }

    Eigen::Matrix4f GetProjectionMatrix() const
    {
        const float FovRadians = Fov * (M_PI / 180.0f);
        const float TanHalfFovY = tan(FovRadians / 2);
        const float ZRange = FarPlane - NearPlane;

        Eigen::Matrix4f ProjMatrix = Eigen::Matrix4f::Zero();
        ProjMatrix(0, 0) = 1.0f / (Aspect * TanHalfFovY);
        ProjMatrix(1, 1) = 1.0f / TanHalfFovY;
        ProjMatrix(2, 2) = -(FarPlane + NearPlane) / ZRange;
        ProjMatrix(2, 3) = -2.0f * FarPlane * NearPlane / ZRange;
        ProjMatrix(3, 2) = -1.0f;

        if (ProjMatrix.array().isNaN().any())
        {
            LOG_ERROR("Warning: Projection matrix contains NaN values!");
        }

        return ProjMatrix;
    }

    void SetPosition(const Eigen::Vector3f& InPosition) override
    {
        Position = InPosition;
    }

    void SetTarget(const Eigen::Vector3f& InTarget)
    {
        Target = InTarget;
    }

    void LookAt(const Eigen::Vector3f& LookAtPoint)
    {
        Target = LookAtPoint;
    }

    void MoveTowards(const Eigen::Vector3f& Direction, float Distance)
    {
        const Eigen::Vector3f NewPos = Position + Direction.normalized() * Distance;
        SetPosition(NewPos);
    }
};