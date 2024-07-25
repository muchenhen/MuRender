#include "VertexShaderInput.h"

StandardVertexShader DefaultStandardVertexShader = [](const VertexShaderInput& Input, const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix)
{
    VertexShaderOutput Output;
    Output.Position = MVPMatrix * Eigen::Vector4f(Input.Position.x(), Input.Position.y(), Input.Position.z(), 1);
    Output.WorldPosition = (ModelMatrix * Eigen::Vector4f(Input.Position.x(), Input.Position.y(), Input.Position.z(), 1)).head<3>();
    Output.UV = Input.UV;
    return Output;
};

NormalVertexShader DefaultNormalVertexShader = [](const VertexShaderInput& Input, const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix, const Eigen::Matrix3f& NormalMatrix)
{
    VertexShaderOutput Output;
    Output.Position = MVPMatrix * Eigen::Vector4f(Input.Position.x(), Input.Position.y(), Input.Position.z(), 1);
    Output.WorldPosition = (ModelMatrix * Eigen::Vector4f(Input.Position.x(), Input.Position.y(), Input.Position.z(), 1)).head<3>();
    Output.UV = Input.UV;
    Output.WorldNormal = NormalMatrix * Input.Normal;
    return Output;
};
