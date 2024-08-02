#include "DirectionalLight.h"

#include "Functions.h"

DirectionalLight::DirectionalLight(Eigen::Vector3f InDirection, Eigen::Vector3f InColor, float InIntensity):
    Direction(std::move(InDirection)), Color(std::move(InColor)), Intensity(InIntensity)
{
}

void DirectionalLight::SetLightSpaceMatrix(const Eigen::Matrix4f& InLightSpaceMatrix)
{
    LightSpaceMatrix = InLightSpaceMatrix;
}

Eigen::Matrix4f DirectionalLight::GetLightSpaceMatrix() const
{
    return LightSpaceMatrix;
}

M4f DirectionalLight::GetLightViewMatrix() const
{
    V3f LightPos = -Direction * 1000.0f;
    V3f Up = Y_AXIS;
    V3f Right = Up.cross(Direction);
    Up = Direction.cross(Right);
    return LookAt(LightPos, ORIGIN, Up);
}

M4f DirectionalLight::GetLightProjectionMatrix(float Left, float Right, float Bottom, float Top, float Near, float Far) const
{
    // 左手系 正交投影
    M4f ProjectionMatrix = M4f::Zero();
    ProjectionMatrix(0, 0) = 2.0f / (Right - Left);
    ProjectionMatrix(1, 1) = 2.0f / (Top - Bottom);
    ProjectionMatrix(2, 2) = -2.0f / (Far - Near);
    ProjectionMatrix(0, 3) = -(Right + Left) / (Right - Left);
    ProjectionMatrix(1, 3) = -(Top + Bottom) / (Top - Bottom);
    ProjectionMatrix(2, 3) = -(Far + Near) / (Far - Near);
    ProjectionMatrix(3, 3) = 1.0f;
    return ProjectionMatrix;
}