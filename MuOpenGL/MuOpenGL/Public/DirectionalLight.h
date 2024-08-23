#pragma once

#include "Object.h"
#include <glm/glm.hpp>

class DirectionalLight : public Object
{
public:
    DirectionalLight(const std::string& name = "DirectionalLight");
    virtual ~DirectionalLight() = default;

    // 设置和获取光照方向
    void SetDirection(const glm::vec3& direction);
    glm::vec3 GetDirection() const;

    // 设置和获取光照颜色
    void SetColor(const glm::vec3& color);
    glm::vec3 GetColor() const;

    // 设置和获取光照强度
    void SetIntensity(float intensity);
    float GetIntensity() const;

    // 启用或禁用光源
    void SetEnabled(bool enabled);
    bool IsEnabled() const;

private:
    glm::vec3 m_direction;
    glm::vec3 m_color;
    float m_intensity;
    bool m_enabled;
};