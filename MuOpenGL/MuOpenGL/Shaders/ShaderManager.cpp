#include "ShaderManager.h"
#include "..\Constants.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>

std::string ShaderManager::LoadShaderSource(const char* filePath)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << '\n';
    }
    return shaderCode;
}

unsigned int ShaderManager::BuildShader(int type)
{
    const unsigned int shader = glCreateShader(type);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << '\n';
    }
    std::filesystem::path shaderSourcePath;
    if (type == GL_VERTEX_SHADER)
    {
        shaderSourcePath = EXECUTABLE_PATH / SHADER_PATH / VERTEX_SHADER_FILE;
    }
    else
    {
        shaderSourcePath = EXECUTABLE_PATH / SHADER_PATH / FRAGMENT_SHADER_FILE;
    }
    const std::string vertexShaderSource = ShaderManager::LoadShaderSource(shaderSourcePath.string().c_str());
    const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
    glShaderSource(shader, 1, &vertexShaderSourceCStr, nullptr);
    glCompileShader(shader);
    // 检查编译错误
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
    }
    return shader;
}

unsigned int ShaderManager::BuildShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    // 着色程序
    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检查链接错误
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << '\n';
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}