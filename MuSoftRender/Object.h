#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "Render.h"

class Object
{
protected:
    Eigen::Vector3f position;
    Eigen::Vector3f rotation;
    Eigen::Vector3f scale;

public:
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
};