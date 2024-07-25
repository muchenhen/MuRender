#include "Object.h"

Object::Object():
    Position(Eigen::Vector3f::Zero()), Rotation(Eigen::Vector3f::Zero()), Scale(Eigen::Vector3f::Ones())
{
}

void Object::Update(float DeltaTime)
{
}

void Object::SetPosition(const Eigen::Vector3f& InPosition)
{
    Position = InPosition;
}

void Object::SetRotation(const Eigen::Vector3f& InRotation)
{
    Rotation = InRotation;
}

void Object::SetScale(const Eigen::Vector3f& InScale)
{
    Scale = InScale;
}

Eigen::Vector3f Object::GetPosition() const
{
    return Position;
}

Eigen::Vector3f Object::GetRotation() const
{
    return Rotation;
}

Eigen::Vector3f Object::GetScale() const
{
    return Scale;
}

Eigen::Matrix4f Object::GetModelMatrix() const
{
    Eigen::Matrix4f Model = Eigen::Matrix4f::Identity();
    Model.block<3, 3>(0, 0) = (Eigen::AngleAxisf(Rotation.x(), Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(Rotation.y(), Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(Rotation.z(), Eigen::Vector3f::UnitZ())).matrix();
    Model.block<3, 1>(0, 3) = Position;
    Model.block<3, 3>(0, 0) *= Scale.asDiagonal();
    return Model;
}

Eigen::Matrix3f Object::GetNormalMatrix() const
{
    return GetModelMatrix().block<3, 3>(0, 0).inverse().transpose();
}