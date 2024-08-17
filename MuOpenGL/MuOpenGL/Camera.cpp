#include "Camera.h"

Camera::Camera(glm::vec3 position) :
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_target(glm::vec3(0.0f, 0.0f, 0.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM)
{
    m_position = position;
    m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_yaw = YAW;
    m_pitch = PITCH;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_position, m_target, m_up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    glm::vec3 movement(0.0f);
    if (direction == FORWARD) movement += m_front * velocity;
    if (direction == BACKWARD) movement -= m_front * velocity;
    if (direction == LEFT) movement -= m_right * velocity;
    if (direction == RIGHT) movement += m_right * velocity;
    
    m_position += movement;
    m_target += movement;  // 移动目标点以保持相对位置
    UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch)
    {
        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    m_zoom -= yoffset;
    m_zoom = glm::clamp(m_zoom, 1.0f, 45.0f);
}

void Camera::LookAt(glm::vec3 target)
{
    m_target = target;
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    m_front = glm::normalize(m_target - m_position);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));

    m_pitch = glm::degrees(asin(m_front.y));
    m_yaw = glm::degrees(atan2(m_front.z, m_front.x));
}