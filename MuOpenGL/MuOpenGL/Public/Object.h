#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

class Object
{
protected:
    std::string m_name;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

    void UpdateModelMatrix() const;

public:
    Object(const std::string& name = "Object");
    virtual ~Object() = default;

    const std::string& GetName() const;
    const glm::vec3& GetPosition() const;
    const glm::quat& GetRotation() const;
    const glm::vec3& GetScale() const;
    glm::mat4 GetModelMatrix() const;

    void SetName(const std::string& name);
    virtual void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    void Translate(const glm::vec3& translation);
    void Rotate(float angleDegrees, const glm::vec3& axis);
    void Scale(const glm::vec3& scale);

    virtual void Update(float deltaTime);
};