#pragma once
#include <Eigen/Core>

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
};

struct VertexShaderOutput
{
    Eigen::Vector4f Position;
    Eigen::Vector2f UV;
    Eigen::Vector3f WorldPosition;
};

using VertexShader = std::function<VertexShaderOutput(const VertexShaderInput&, const Eigen::Matrix4f&, const Eigen::Matrix4f&)>;

extern VertexShader DefaultVertexShader;