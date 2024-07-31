#include "Camera.h"

Camera::Camera(const Eigen::Vector3f& InPosition, const Eigen::Vector3f& InTarget, const Eigen::Vector3f& InUpVector, float InFieldOfView, float InAspectRatio, float InNearValue, float InFarValue) :
    Object(),
    Fov(InFieldOfView),
    Aspect(InAspectRatio),
    NearPlane(InNearValue),
    FarPlane(InFarValue),
    Up(InUpVector),
    Target(InTarget)
{
    Camera::SetPosition(InPosition);
}

Eigen::Matrix4f Camera::GetViewMatrix() const
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

Eigen::Matrix4f Camera::GetProjectionMatrix() const
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

void Camera::SetPosition(const Eigen::Vector3f& InPosition)
{
    Position = InPosition;
}

void Camera::SetTarget(const Eigen::Vector3f& InTarget)
{
    Target = InTarget;
}

void Camera::SetYaw(float InYaw)
{
    Yaw = InYaw;
}

void Camera::SetPitch(float InPitch)
{
    Pitch = InPitch;
}

void Camera::LookAt(const Eigen::Vector3f& LookAtPoint)
{
    Target = LookAtPoint;
}

void Camera::MoveTowards(const Eigen::Vector3f& Direction, float Distance)
{
    const Eigen::Vector3f NewPos = Position + Direction.normalized() * Distance;
    SetPosition(NewPos);
}

Eigen::Vector3f Camera::GetForward() const
{
    return (Target - Position).normalized();
}

Eigen::Vector3f Camera::GetRight() const
{
    return GetForward().cross(Up).normalized();
}

Eigen::Vector3f Camera::GetUp() const
{
    return GetRight().cross(GetForward()).normalized();
}

Eigen::Vector3f Camera::GetPosition() const
{
    return Position;
}

void Camera::AddYaw(float InYaw)
{
    Yaw += InYaw;
    UpdateVectors();
}

void Camera::AddPitch(float InPitch)
{
    Pitch += InPitch;
    UpdateVectors();
}

void Camera::ClampPitch(float MinPitch, float MaxPitch)
{
    Pitch = std::clamp(Pitch, MinPitch, MaxPitch);
    UpdateVectors();
}

void Camera::UpdateVectors()
{
    // Eigen::Vector3f Direction;
    // auto radYaw = DegToRad(Yaw);
    // auto radPitch = DegToRad(Pitch);
    // Direction.x() = cos(radYaw) * cos(radPitch);
    // Direction.y() = sin(radPitch);
    // Direction.z() = sin(radYaw) * cos(radPitch);
    // Direction.normalize();
    //
    // Target = Position + Direction;
    // Up = GetRight().cross(Direction).normalized();
}