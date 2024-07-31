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

    Object();

    virtual void Update(float DeltaTime);

    virtual void SetPosition(const Eigen::Vector3f& InPosition);

    void SetRotation(const Eigen::Vector3f& InRotation);

    void SetScale(const Eigen::Vector3f& InScale);

    virtual Eigen::Vector3f GetPosition() const;

    Eigen::Vector3f GetRotation() const;

    Eigen::Vector3f GetScale() const;

    virtual Eigen::Matrix4f GetModelMatrix() const;

    virtual Eigen::Matrix3f GetNormalMatrix() const;
};