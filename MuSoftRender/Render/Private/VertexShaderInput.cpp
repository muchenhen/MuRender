#include "VertexShaderInput.h"

VertexShader DefaultVertexShader = [](const VertexShaderInput& Input, const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix)
{
    VertexShaderOutput Output;
    Output.Position = MVPMatrix * Eigen::Vector4f(Input.Position.x(), Input.Position.y(), Input.Position.z(), 1);
    Output.WorldPosition = (ModelMatrix * Eigen::Vector4f(Input.Position.x(), Input.Position.y(), Input.Position.z(), 1)).head<3>();
    Output.UV = Input.UV;
    return Output;
};
