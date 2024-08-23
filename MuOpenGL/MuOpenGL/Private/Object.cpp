#include "Object.h"

void Object::UpdateModelMatrix() const
{
}

Object::Object(const std::string& name):
    m_position(),
    m_rotation(),
    m_scale(1, 1, 1)
{
    m_name = name;
}

const std::string& Object::GetName() const
{
    return m_name;
}

const glm::vec3& Object::GetPosition() const
{
    return m_position;
}

const glm::quat& Object::GetRotation() const
{
    return m_rotation;
}

const glm::vec3& Object::GetScale() const
{
    return m_scale;
}

glm::mat4 Object::GetModelMatrix() const
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = translate(modelMatrix, m_position);
    modelMatrix = modelMatrix * mat4_cast(m_rotation);
    modelMatrix = scale(modelMatrix, m_scale);
    return modelMatrix;
}

void Object::SetName(const std::string& name)
{
    m_name = name;
}

void Object::SetPosition(const glm::vec3& position)
{
    m_position = position;
    UpdateModelMatrix();
}

void Object::SetRotation(const glm::quat& rotation)
{
    m_rotation = rotation;
    UpdateModelMatrix();
}

void Object::SetScale(const glm::vec3& scale)
{
    m_scale = scale;
    UpdateModelMatrix();
}

void Object::Translate(const glm::vec3& translation)
{
    m_position += translation;
}

/**
 * 
 * @param angleDegrees 角度
 * @param axis 
 */
void Object::Rotate(float angleDegrees, const glm::vec3& axis)
{
    float angleRadians = glm::radians(angleDegrees);
    m_rotation = glm::rotate(m_rotation, angleRadians, glm::normalize(axis));
}

void Object::Scale(const glm::vec3& scale)
{
    m_scale *= scale;
}

void Object::Update(double deltaTime)
{
}