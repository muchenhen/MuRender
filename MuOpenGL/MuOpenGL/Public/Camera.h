#pragma once

#include "Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Object
{
private:
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

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

    Camera(const std::string& name = "Camera",
           const glm::vec3& position = glm::vec3(0.0f),
           const glm::vec3& target = glm::vec3(0.0f, 0.0f, -1.0f),
           const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    const glm::vec3& GetFront() const;
    const glm::vec3& GetUp() const;
    const glm::vec3& GetRight() const;
    float GetFOV() const;

    void SetTarget(const glm::vec3& target);
    void SetFOV(float fov);
    void SetAspectRatio(float aspectRatio);
    void SetClipPlanes(float nearPlane, float farPlane);

    void SetPosition(const glm::vec3& position) override;

    void Move(MovementDirection direction, float deltaTime);
    void Rotate(float yaw, float pitch);
    void Zoom(float yOffset);

    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

    void Update(float deltaTime) override;
};