#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    glm::vec3 m_position;
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

    Camera(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    // Getters
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetFront() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;
    float GetFOV() const;

    // Setters
    void SetPosition(const glm::vec3& position);
    void SetTarget(const glm::vec3& target);
    void SetFOV(float fov);

    // Movement
    void Move(MovementDirection direction, float deltaTime);
    void Rotate(RotationType type, float angle);
    void Zoom(float yOffset);

    // Mouse input
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
};