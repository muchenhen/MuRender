#pragma once
#include "Functions.h"
#include "Logger.h"
#include "Object.h"

class Camera : public Object
{
private:
    float Fov;
    float Aspect;
    float NearPlane;
    float FarPlane;
    Eigen::Vector3f Up;
    Eigen::Vector3f Target;

    float Yaw;
    float Pitch;

public:
    Camera(const Eigen::Vector3f& InPosition,
           const Eigen::Vector3f& InTarget,
           const Eigen::Vector3f& InUpVector,
           float InFieldOfView, float InAspectRatio, float InNearValue, float InFarValue);

    Eigen::Matrix4f GetViewMatrix() const;

    Eigen::Matrix4f GetProjectionMatrix() const;

public:
    void SetPosition(const Eigen::Vector3f& InPosition) override;

    void SetTarget(const Eigen::Vector3f& InTarget);

    void SetYaw(float InYaw);

    void SetPitch(float InPitch);

    void LookAt(const Eigen::Vector3f& LookAtPoint);

    void MoveTowards(const Eigen::Vector3f& Direction, float Distance);

public:
    Eigen::Vector3f GetForward() const;

    Eigen::Vector3f GetRight() const;

    Eigen::Vector3f GetUp() const;

    Eigen::Vector3f GetPosition() const override;

    std::vector<float> GetFrustumPlanes() const;
    
public:
    void AddYaw(float InYaw);
    void AddPitch(float InPitch);
    void ClampPitch(float MinPitch, float MaxPitch);
    void UpdateVectors();

    static float DegToRad(float degrees)
    {
        return degrees * (M_PI / 180.0f);
    }
};