#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string& name) :
    Object(name),
    m_direction(0.0f, -1.0f, 0.0f), // 默认方向：向下
    m_color(1.0f, 1.0f, 1.0f),      // 默认颜色：白色
    m_intensity(1.0f),              // 默认强度：1.0
    m_enabled(true)                 // 默认启用
{
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
    m_direction = glm::normalize(direction);
}

glm::vec3 DirectionalLight::GetDirection() const
{
    return m_direction;
}

void DirectionalLight::SetColor(const glm::vec3& color)
{
    m_color = color;
}

glm::vec3 DirectionalLight::GetColor() const
{
    return m_color;
}

void DirectionalLight::SetIntensity(float intensity)
{
    m_intensity = intensity;
}

float DirectionalLight::GetIntensity() const
{
    return m_intensity;
}

void DirectionalLight::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool DirectionalLight::IsEnabled() const
{
    return m_enabled;
}