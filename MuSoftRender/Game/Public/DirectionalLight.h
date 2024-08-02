#pragma once
#include <utility>

#include "CoordinateSystem .h"
#include "Object.h"

class DirectionalLight : public Object
{
public:
    Eigen::Vector3f Direction;
    Eigen::Vector3f Color;
    float Intensity;

    Eigen::Matrix4f LightSpaceMatrix;

    DirectionalLight(Eigen::Vector3f InDirection, Eigen::Vector3f InColor, float InIntensity);

    void SetLightSpaceMatrix(const Eigen::Matrix4f& InLightSpaceMatrix);

    Eigen::Matrix4f GetLightSpaceMatrix() const;

    M4f GetLightViewMatrix() const;

    M4f GetLightProjectionMatrix(float Left, float Right, float Bottom, float Top, float Near, float Far) const;
};