#pragma once
#include <Eigen/Core>

#include "Material.h"

struct FragmentShaderInput
{
    Eigen::Vector2f UV;
    Eigen::Vector3f WorldPosition;
};

using FragmentShader = std::function<Eigen::Vector4f(const FragmentShaderInput&, const Material*)>;

extern FragmentShader DefaultFragmentShader;