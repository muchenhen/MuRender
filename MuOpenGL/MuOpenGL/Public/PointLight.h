#pragma once
#include "Object.h"

class PointLight : public Object
{
public:
    PointLight(const std::string& name = "PointLight");

    void SetColor(const glm::vec3& color) { m_color = color; }
    void SetIntensity(float intensity) { m_intensity = intensity; }

    void SetAttenuation(float constant, float linear, float quadratic)
    {
        m_constant = constant;
        m_linear = linear;
        m_quadratic = quadratic;
    }

    glm::vec3 GetColor() const;
    float GetIntensity() const;
    float GetConstant() const;
    float GetLinear() const;
    float GetQuadratic() const;

private:
    glm::vec3 m_color;
    float m_intensity;
    float m_constant;  // 常数项
    float m_linear;    // 一次项
    float m_quadratic; // 二次项
};