#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(const std::string& name, const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) :
    Object(name), m_worldUp(up), m_fov(45.0f), m_aspectRatio(1.0f), m_nearPlane(0.1f), m_farPlane(100.0f),
    m_movementSpeed(2.5f), m_mouseSensitivity(0.1f)
{
    Camera::SetPosition(position);
    SetTarget(target);
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    m_front = glm::normalize(glm::rotate(m_rotation, glm::vec3(0.0f, 0.0f, -1.0f)));
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

const glm::vec3& Camera::GetFront() const
{
    return m_front;
}

const glm::vec3& Camera::GetUp() const
{
    return m_up;
}

const glm::vec3& Camera::GetRight() const
{
    return m_right;
}

float Camera::GetFOV() const
{
    return m_fov;
}

void Camera::OrbitAroundTarget(float angleInDegrees)
{
    // 将角度转换为弧度
    float angleInRadians = glm::radians(angleInDegrees);

    // 计算当前相对于目标的位置
    glm::vec3 relativePos = m_position - m_target;

    // 创建旋转矩阵
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angleInRadians, m_worldUp);

    // 应用旋转
    relativePos = glm::vec3(rotation * glm::vec4(relativePos, 1.0f));

    // 更新摄像机位置
    m_position = m_target + relativePos;

    // 更新摄像机朝向
    SetTarget(m_target);
}

void Camera::SetTarget(const glm::vec3& target)
{
    m_target = target;
    m_rotation = glm::quatLookAt(glm::normalize(target - m_position), m_worldUp);
    UpdateCameraVectors();
}

const glm::vec3& Camera::GetTarget() const
{
    return m_target;
}

void Camera::SetFOV(float fov)
{
    m_fov = fov;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
}

void Camera::SetClipPlanes(float nearPlane, float farPlane)
{
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::SetPosition(const glm::vec3& position)
{
    Object::SetPosition(position);
    UpdateCameraVectors();
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
        case MovementDirection::Up: m_position += m_worldUp * velocity;
            break;
        case MovementDirection::Down: m_position -= m_worldUp * velocity;
            break;
    }
}

void Camera::Rotate(float yaw, float pitch)
{
    glm::quat qPitch = glm::angleAxis(glm::radians(pitch), m_right);
    glm::quat qYaw = glm::angleAxis(glm::radians(yaw), m_worldUp);
    m_rotation = qYaw * m_rotation * qPitch;
    UpdateCameraVectors();
}

void Camera::Zoom(float yOffset)
{
    if (m_fov >= 1.0f && m_fov <= 45.0f) m_fov -= yOffset;
    if (m_fov <= 1.0f) m_fov = 1.0f;
    if (m_fov >= 45.0f) m_fov = 45.0f;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
}

void Camera::Update(double deltaTime)
{
    // 如果需要，在这里添加任何每帧更新逻辑
    UpdateCameraVectors();
}