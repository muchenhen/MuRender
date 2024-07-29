#pragma once
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)

#include "DirectionalLight.h"
#include "Material.h"

struct FragmentShaderInput
{
    Eigen::Vector2f UV;
    Eigen::Vector3f WorldPosition;
    Eigen::Vector3f WorldNormal;
};

using FragmentShader = std::function<Eigen::Vector4f(const FragmentShaderInput&, const Material*)>;

extern FragmentShader DefaultFragmentShader;

using SimpleLitFragmentShader = std::function<Eigen::Vector4f(const FragmentShaderInput&, const Material*, const DirectionalLight*)>;

extern SimpleLitFragmentShader DefaultSimpleLitFragmentShader;