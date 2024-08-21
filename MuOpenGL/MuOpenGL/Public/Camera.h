#pragma once

#include "Object.h"

class Camera : public Object
{
private:
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;
    float m_roll;
    float m_fov;

    float m_movementSpeed;
    float m_mouseSensitivity;

    void UpdateCameraVectors();

public:
    enum class MovementDirection : std::uint8_t
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    enum class RotationType : std::uint8_t
    {
        Pitch,
        Yaw,
        Roll
    };

    Camera(const std::string& name = "Camera",
           const glm::vec3& position = glm::vec3(0.0f),
           const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f),
           const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetFront() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;
    float GetFOV() const;

    void SetTarget(const glm::vec3& target);
    void SetFOV(float fov);

    void SetPosition(const glm::vec3& position) override;
    
    void Move(MovementDirection direction, float deltaTime);
    void Rotate(RotationType type, float angle);
    void Zoom(float yOffset);

    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
};