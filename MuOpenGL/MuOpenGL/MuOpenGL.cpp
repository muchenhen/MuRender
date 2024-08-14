#include <filesystem>

#include "Window.h"
#include <iostream>

#include "Shaders/ShaderManager.h"
#include "Constants.h"

int main(int argc, char* argv[])
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

    auto vertexShader = ShaderManager::BuildShader(GL_VERTEX_SHADER);
    auto fragmentShader = ShaderManager::BuildShader(GL_FRAGMENT_SHADER);
    // 着色程序
    unsigned int shaderProgram = glCreateProgram();
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

    // 顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    // 顶点缓冲对象
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 绑定VAO
    glBindVertexArray(VAO);
    // 绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 渲染循环
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    window.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!window.ShouldClose())
    {
        window.ProcessInput();

        window.Clear();

        // 绘制三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.SwapBuffersAndPollEvents();
    }

    // 清理
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    window.Terminate();

    return 0;
}