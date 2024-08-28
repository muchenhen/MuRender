#pragma once
#include "Object.h"

class PointLight : public Object
{
public:
    PointLight(const std::string& name = "PointLight");

    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);

    void SetAttenuation(float constant, float linear, float quadratic);

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