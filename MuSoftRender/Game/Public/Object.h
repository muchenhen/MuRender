#pragma once
#pragma warning(disable : 4819)
#include <Eigen/Core>
#include <Eigen/Geometry>
#pragma warning(default : 4819)

class Object
{
protected:
    Eigen::Vector3f Position;
    Eigen::Vector3f Rotation;
    Eigen::Vector3f Scale;

public:
    virtual ~Object() = default;

    Object() :
        Position(Eigen::Vector3f::Zero()), Rotation(Eigen::Vector3f::Zero()), Scale(Eigen::Vector3f::Ones())
    {
    }

    virtual void Update(float DeltaTime)
    {
    }

    virtual void SetPosition(const Eigen::Vector3f& InPosition)
    {
        Position = InPosition;
    }

    void SetRotation(const Eigen::Vector3f& InRotation)
    {
        Rotation = InRotation;
    }

    void SetScale(const Eigen::Vector3f& InScale)
    {
        Scale = InScale;
    }

    Eigen::Vector3f GetPosition() const
    {
        return Position;
    }

    Eigen::Vector3f GetRotation() const
    {
        return Rotation;
    }

    Eigen::Vector3f GetScale() const
    {
        return Scale;
    }

    virtual Eigen::Matrix4f GetModelMatrix() const
    {
        Eigen::Matrix4f Model = Eigen::Matrix4f::Identity();
        Model.block<3, 3>(0, 0) = (Eigen::AngleAxisf(Rotation.x(), Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(Rotation.y(), Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(Rotation.z(), Eigen::Vector3f::UnitZ())).matrix();
        Model.block<3, 1>(0, 3) = Position;
        Model.block<3, 3>(0, 0) *= Scale.asDiagonal();
        return Model;
    }

    virtual Eigen::Matrix3f GetNormalMatrix() const;

};