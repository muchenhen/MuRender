#include "PointLight.h"

PointLight::PointLight(const std::string& name):
    m_color(1.0f, 1.0f, 1.0f),
    m_intensity(1.0f),
    m_constant(0),
    m_linear(0),
    m_quadratic(0)
{
}

glm::vec3 PointLight::GetColor() const
{
    return m_color;
}

float PointLight::GetIntensity() const
{
    return m_intensity;
}

float PointLight::GetConstant() const
{
    return m_constant;
}

float PointLight::GetLinear() const
{
    return m_linear;
}

float PointLight::GetQuadratic() const
{
    return m_quadratic;
}