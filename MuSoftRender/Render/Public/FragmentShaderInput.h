#pragma once
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)

#include "Material.h"

struct FragmentShaderInput
{
    Eigen::Vector2f UV;
    Eigen::Vector3f WorldPosition;
};

using FragmentShader = std::function<Eigen::Vector4f(const FragmentShaderInput&, const Material*)>;

extern FragmentShader DefaultFragmentShader;