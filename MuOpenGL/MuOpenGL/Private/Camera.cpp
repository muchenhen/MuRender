#include "Camera.h"
#include <algorithm>

#include "Constants.h"


void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = normalize(front);
    m_right = normalize(cross(m_front, m_worldUp));
    m_up = normalize(cross(m_right, m_front));
}

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) :
    m_position(position), m_worldUp(up), m_yaw(YAW), m_pitch(PITCH), m_roll(0.0f),
    m_fov(FOV), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY)
{
    SetTarget(target);
}

glm::mat4 Camera::GetViewMatrix() const
{
    return lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::GetPosition() const
{
    return m_position;
}

glm::vec3 Camera::GetFront() const
{
    return m_front;
}

glm::vec3 Camera::GetUp() const
{
    return m_up;
}

glm::vec3 Camera::GetRight() const
{
    return m_right;
}

float Camera::GetFOV() const
{
    return m_fov;
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    UpdateCameraVectors();
}

void Camera::SetTarget(const glm::vec3& target)
{
    m_front = normalize(target - m_position);
    m_right = normalize(cross(m_front, m_worldUp));
    m_up = normalize(cross(m_right, m_front));

    m_yaw = glm::degrees(std::atan2(m_front.z, m_front.x));
    m_pitch = glm::degrees(std::asin(m_front.y));
}

void Camera::SetFOV(float fov)
{
    m_fov = std::clamp(fov, 1.0f, 90.0f);
}

void Camera::Move(MovementDirection direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    switch (direction)
    {
        case MovementDirection::Forward: m_position += m_front * velocity;
            break;
        case MovementDirection::Backward: m_position -= m_front * velocity;
            break;
        case MovementDirection::Left: m_position -= m_right * velocity;
            break;
        case MovementDirection::Right: m_position += m_right * velocity;
            break;
        case MovementDirection::Up: m_position += m_up * velocity;
            break;
        case MovementDirection::Down: m_position -= m_up * velocity;
            break;
    }
}

void Camera::Rotate(RotationType type, float angle)
{
    switch (type)
    {
        case RotationType::Pitch: m_pitch += angle;
            break;
        case RotationType::Yaw: m_yaw += angle;
            break;
        case RotationType::Roll: m_roll += angle;
            break;
    }
    UpdateCameraVectors();
}

void Camera::Zoom(float yOffset)
{
    m_fov -= yOffset;
    m_fov = std::clamp(m_fov, 1.0f, 90.0f);
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (constrainPitch)
    {
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}