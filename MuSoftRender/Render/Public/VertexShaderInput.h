#pragma once
#include <Eigen/Core>

struct Vertex
{
    Eigen::Vector3f Position;
    Eigen::Vector2f UV;
    Eigen::Vector3f Normal;

    Vertex() = default;

    Vertex(Eigen::Vector3f InPosition) :
        Position(std::move(InPosition))
    {
    }

    Vertex(Eigen::Vector3f InPosition, Eigen::Vector2f InUV) :
        Position(std::move(InPosition)),
        UV(std::move(InUV))
    {
    }

    Vertex(Eigen::Vector3f InPosition, Eigen::Vector2f InUV, Eigen::Vector3f InNormal) :
        Position(std::move(InPosition)),
        UV(std::move(InUV)),
        Normal(std::move(InNormal))
    {
    }
};

struct VertexShaderInput
{
    Eigen::Vector3f Position; // clip space
    Eigen::Vector3f Normal;
    Eigen::Vector2f UV;
};

struct VertexShaderOutput
{
    Eigen::Vector4f Position;
    Eigen::Vector2f UV;
    Eigen::Vector3f WorldPosition;
    Eigen::Vector3f WorldNormal;
};

using VertexShader = std::function<VertexShaderOutput(const VertexShaderInput&, const Eigen::Matrix4f&, const Eigen::Matrix4f&)>;

extern VertexShader DefaultVertexShader;

using NormalVertexShader = std::function<VertexShaderOutput(const VertexShaderInput&, const Eigen::Matrix4f&, const Eigen::Matrix4f&, const Eigen::Matrix3f&)>;

extern NormalVertexShader DefaultNormalVertexShader;

template <typename T>
class VertexShaderWrapper
{
private:
    T Shader;
    Eigen::Matrix3f NormalMatrix; // 只用在NormalVertexShader

public:
    VertexShaderWrapper(T s):
        Shader(std::move(s))
    {
    }

    void SetNormalMatrix(const Eigen::Matrix3f& InNormalMatrix)
    {
        NormalMatrix = InNormalMatrix;
    }

    VertexShaderOutput operator()(const VertexShaderInput& Input, const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix) const
    {
        if constexpr (std::is_same<T, VertexShader>::value)
        {
            return Shader(Input, ModelMatrix, MVPMatrix);
        }
        else if constexpr (std::is_same<T, NormalVertexShader>::value)
        {
            return Shader(Input, ModelMatrix, MVPMatrix, NormalMatrix);
        }
    }
};