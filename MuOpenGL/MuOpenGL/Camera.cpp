#include "Camera.h"
#include <algorithm>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
    : m_position(position), m_worldUp(up), m_yaw(YAW), m_pitch(PITCH), m_roll(0.0f),
    m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fov(FOV)
{
    SetTarget(target);
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    UpdateCameraVectors();
}

void Camera::SetTarget(const glm::vec3& target)
{
    m_front = glm::normalize(target - m_position);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
    
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
    switch (direction) {
        case MovementDirection::Forward:
            m_position += m_front * velocity;
            break;
        case MovementDirection::Backward:
            m_position -= m_front * velocity;
            break;
        case MovementDirection::Left:
            m_position -= m_right * velocity;
            break;
        case MovementDirection::Right:
            m_position += m_right * velocity;
            break;
        case MovementDirection::Up:
            m_position += m_up * velocity;
            break;
        case MovementDirection::Down:
            m_position -= m_up * velocity;
            break;
    }
}

void Camera::Rotate(RotationType type, float angle)
{
    switch (type) {
        case RotationType::Pitch:
            m_pitch += angle;
            break;
        case RotationType::Yaw:
            m_yaw += angle;
            break;
        case RotationType::Roll:
            m_roll += angle;
            break;
    }
    UpdateCameraVectors();
}

void Camera::Zoom(float yoffset)
{
    m_fov -= yoffset;
    m_fov = std::clamp(m_fov, 1.0f, 90.0f);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}