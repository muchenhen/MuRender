#pragma once
#include <utility>

#include "CoordinateSystem .h"
#include "Object.h"
#include "BoundingBox.h"
#include "DepthTexture.h"

class DirectionalLight : public Object
{
public:
    Eigen::Vector3f Direction;
    Eigen::Vector3f Color;
    float Intensity;

    DepthTexture* ShadowMap;


    Eigen::Matrix4f LightSpaceMatrix;

    DirectionalLight(Eigen::Vector3f InDirection, Eigen::Vector3f InColor, float InIntensity);

    void SetLightSpaceMatrix(const Eigen::Matrix4f& InLightSpaceMatrix);

    Eigen::Matrix4f GetLightSpaceMatrix() const;

    M4f GetLightViewMatrix() const;

    // 计算光源视图矩阵
    M4f GetLightViewMatrix(const BoundingBox& SceneBounds) const;

    // 计算光源正交投影矩阵
    M4f CreateOrthoProjection(float Left, float Right, float Bottom, float Top, float Near, float Far) const;

    // 计算LightProjectionMatrix
    M4f GetLightProjectionMatrix(const BoundingBox& SceneBounds, const M4f& LightViewMatrix) const;

    void CreateShadowMap(int Width, int Height);
};