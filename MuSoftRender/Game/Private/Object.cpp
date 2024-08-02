#include "Object.h"

#include "CoordinateSystem .h"

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
    M4f S = M4f::Identity();
    S.block<3, 3>(0, 0) = this->Scale.asDiagonal();

    M4f R = M4f::Identity();
    auto RotationX = Eigen::AngleAxisf(this->Rotation.x(), X_AXIS);
    auto RotationY = Eigen::AngleAxisf(this->Rotation.y(), Y_AXIS);
    auto RotationZ = Eigen::AngleAxisf(this->Rotation.z(), Z_AXIS);
    R.block<3, 3>(0, 0) = (RotationZ * RotationY * RotationX).matrix();

    M4f T = M4f::Identity();
    T.block<3, 1>(0, 3) = Position;

    Eigen::Matrix4f Model = T * R * S;
    return Model;
}

Eigen::Matrix3f Object::GetNormalMatrix() const
{
    return GetModelMatrix().block<3, 3>(0, 0).inverse().transpose();
}