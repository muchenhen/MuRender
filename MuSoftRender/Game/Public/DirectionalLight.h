#pragma once
#include <utility>

#include "Object.h"

class DirectionalLight : public Object
{
public:
    Eigen::Vector3f Direction;
    Eigen::Vector3f Color;
    float Intensity;

    DirectionalLight(Eigen::Vector3f InDirection, Eigen::Vector3f InColor, float InIntensity) :
        Direction(std::move(InDirection)), Color(std::move(InColor)), Intensity(InIntensity)
    {
    }
};