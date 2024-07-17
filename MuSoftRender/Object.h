#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

class Object
{
protected:
    Eigen::Vector3f position;
    Eigen::Vector3f rotation;
    Eigen::Vector3f scale;

public:
    virtual ~Object() = default;

    Object() :
        position(Eigen::Vector3f::Zero()), rotation(Eigen::Vector3f::Zero()), scale(Eigen::Vector3f::Ones())
    {
    }

    virtual void Update(float deltaTime)
    {

    }

    void SetPosition(const Eigen::Vector3f& pos)
    {
        position = pos;
    }
    void SetRotation(const Eigen::Vector3f& rot)
    {
        rotation = rot;
    }
    void SetScale(const Eigen::Vector3f& scl)
    {
        scale = scl;
    }

    Eigen::Vector3f GetPosition() const
    {
        return position;
    }
    Eigen::Vector3f GetRotation() const
    {
        return rotation;
    }
    Eigen::Vector3f GetScale() const
    {
        return scale;
    }

    virtual Eigen::Matrix4f GetModelMatrix() const
    {
        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
        // 应用旋转
        model.block<3, 3>(0, 0) = (Eigen::AngleAxisf(rotation.x(), Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(rotation.y(), Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(rotation.z(), Eigen::Vector3f::UnitZ())).matrix();
        // 应用位置
        model.block<3, 1>(0, 3) = position;
        // 应用缩放
        model.block<3, 3>(0, 0) *= scale.asDiagonal();
        return model;
    }
};