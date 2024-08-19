#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    enum class MovementDirection
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    enum class RotationType
    {
        Pitch,
        Yaw,
        Roll
    };

    Camera(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    // Getters
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetFront() const { return m_front; }
    glm::vec3 GetUp() const { return m_up; }
    glm::vec3 GetRight() const { return m_right; }
    float GetFOV() const { return m_fov; }

    // Setters
    void SetPosition(const glm::vec3& position);
    void SetTarget(const glm::vec3& target);
    void SetFOV(float fov);

    // Movement
    void Move(MovementDirection direction, float deltaTime);
    void Rotate(RotationType type, float angle);
    void Zoom(float yoffset);

    // Mouse input
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

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
};