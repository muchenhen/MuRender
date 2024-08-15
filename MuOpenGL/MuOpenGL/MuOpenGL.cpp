#include "Window.h"

#include "Shaders/ShaderManager.h"
#include "Constants.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

int main(int argc, char* argv[])
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

    Shader shader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());

    // 设置顶点数据
    std::vector<Vertex> vertices = {
        {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
    };

    std::vector<unsigned int> indices = {
        0, 1, 3,
        1, 2, 3
    };

    Mesh quad(vertices, indices);

    unsigned int texture1;
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    Texture::BuildTexture("NagisaKaworu.bmp", texture1, width, height, nrChannels);

    window.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    shader.Use();
    glUniform1i(glGetUniformLocation(shader.ID, "texture"), 0);

    while (!window.ShouldClose())
    {
        // 处理输入
        window.ProcessInput();
        // 渲染
        window.Clear();

        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // 绘制三角形
        quad.Draw(shader);

        window.SwapBuffersAndPollEvents();
    }

    // 清理
    glDeleteProgram(shader.ID);

    window.Terminate();

    return 0;
}