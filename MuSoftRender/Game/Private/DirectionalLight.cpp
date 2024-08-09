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
    V3f Up = (std::abs(Direction.dot(Y_AXIS)) > 0.99f) ? X_AXIS : Y_AXIS;
    V3f Right = Up.cross(Direction).normalized();
    Up = Direction.cross(Right).normalized();
    return LookAt(LightPos, ORIGIN, Up);
}

M4f DirectionalLight::GetLightViewMatrix(const BoundingBox& SceneBounds) const
{
    V3f Center = SceneBounds.GetCenter();
    V3f LightPos = Center + Direction * SceneBounds.GetDiagonalLength();
    
    // 选择一个不与光照方向平行的向量作为初始 Up 向量
    V3f Up = (std::abs(Direction.dot(Y_AXIS)) > 0.99f) ? Z_AXIS : Y_AXIS;
    
    V3f Right = Up.cross(Direction).normalized();
    Up = Direction.cross(Right).normalized();
    
    return LookAt(LightPos, Center, Up);
}

M4f DirectionalLight::CreateOrthoProjection(float Left, float Right, float Bottom, float Top, float Near, float Far) const
{
    // 左手系 正交投影
    M4f ProjectionMatrix = M4f::Zero();
    ProjectionMatrix(0, 0) = 2.0f / (Right - Left);
    ProjectionMatrix(1, 1) = 2.0f / (Top - Bottom);
    ProjectionMatrix(2, 2) = 1.0f / (Far - Near);
    ProjectionMatrix(0, 3) = -(Right + Left) / (Right - Left);
    ProjectionMatrix(1, 3) = -(Top + Bottom) / (Top - Bottom);
    ProjectionMatrix(2, 3) = -Near / (Far - Near);
    ProjectionMatrix(3, 3) = 1.0f;
    return ProjectionMatrix;
}

M4f DirectionalLight::GetLightProjectionMatrix(const BoundingBox& SceneBounds, const M4f& LightViewMatrix) const
{
    std::vector<V3f> Corners = SceneBounds.GetCorners();
    V3f minBounds = V3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    V3f maxBounds = V3f(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    for (const V3f& Corner : Corners)
    {
        V4f TransformedCorner = LightViewMatrix * Corner.homogeneous();
        minBounds = minBounds.cwiseMin(TransformedCorner.head<3>());
        maxBounds = maxBounds.cwiseMax(TransformedCorner.head<3>());
    }

    float padding = (maxBounds.x() - minBounds.x()) * 0.1f;

    float Left = minBounds.x() - padding;
    float Right = maxBounds.x() + padding;
    float Bottom = minBounds.y() - padding;
    float Top = maxBounds.y() + padding;
    float Near = minBounds.z() - padding;
    float Far = maxBounds.z() + padding;

    M4f LightProjectionMatrix = CreateOrthoProjection(Left, Right, Bottom, Top, Near, Far);
    return LightProjectionMatrix;
}

void DirectionalLight::CreateShadowMap(int Width, int Height)
{
    ShadowMap = new DepthTexture(Width, Height);
}