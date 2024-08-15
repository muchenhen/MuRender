#include "Shader.h"

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