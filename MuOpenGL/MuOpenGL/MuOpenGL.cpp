#include "Window.h"

#include "Shaders/ShaderManager.h"
#include "Constants.h"

int main(int argc, char* argv[])
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

    const auto vertexShader = ShaderManager::BuildShader(GL_VERTEX_SHADER);
    const auto fragmentShader = ShaderManager::BuildShader(GL_FRAGMENT_SHADER);
    const auto shaderProgram = ShaderManager::BuildShaderProgram(vertexShader, fragmentShader);

    // 顶点数据
    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    // 顶点缓冲对象
    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    // 绑定VAO
    glBindVertexArray(vao);
    // 绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
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
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.SwapBuffersAndPollEvents();
    }

    // 清理
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    window.Terminate();

    return 0;
}