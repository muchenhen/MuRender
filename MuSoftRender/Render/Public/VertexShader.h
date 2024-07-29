#pragma once
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)

struct Vertex
{
    Eigen::Vector3f Position;
    Eigen::Vector2f UV;
    Eigen::Vector3f Normal;
};

struct VertexShaderInput
{
    Eigen::Vector3f Position;// clip space
    Eigen::Vector2f UV;
    Eigen::Vector3f Normal;
};

struct VertexShaderOutput
{
    Eigen::Vector4f Position;
    Eigen::Vector2f UV;
    Eigen::Vector3f WorldPosition;
    Eigen::Vector3f WorldNormal;
};

using StandardVertexShader = std::function<VertexShaderOutput(const VertexShaderInput&, const Eigen::Matrix4f&, const Eigen::Matrix4f&)>;

extern StandardVertexShader DefaultStandardVertexShader;

using NormalVertexShader = std::function<VertexShaderOutput(const VertexShaderInput&, const Eigen::Matrix4f&, const Eigen::Matrix4f&, const Eigen::Matrix3f&)>;

extern NormalVertexShader DefaultNormalVertexShader;