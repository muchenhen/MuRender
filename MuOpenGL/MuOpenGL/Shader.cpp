#include "Shader.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "Shaders/ShaderManager.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    const auto vertexShader = ShaderManager::BuildShader(GL_VERTEX_SHADER, vertexPath);
    const auto fragmentShader = ShaderManager::BuildShader(GL_FRAGMENT_SHADER, fragmentPath);

    ID = ShaderManager::BuildShaderProgram(vertexShader, fragmentShader);
}

void Shader::Use() const
{
    glUseProgram(ID);
}

// TODO:
void Shader::SetBool(const char* name, bool value) const
{
}

void Shader::SetInt(const char* name, int value) const
{
}

void Shader::SetFloat(const char* name, float value) const
{
}

void Shader::SetMat4(const char* name, glm::mat4 value) const
{
    GLint location = glGetUniformLocation(ID, name);
    if (location == -1)
    {
        std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << '\n';
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}