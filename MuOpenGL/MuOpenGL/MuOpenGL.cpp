#include "Window.h"

#include "Constants.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "InputManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }

int main(int argc, char* argv[])
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    InputManager& inputManager = InputManager::GetInstance();
    inputManager.Initialize(window.GetWindow());
    inputManager.SetCamera(camera);

    Shader shader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());

    std::vector<Vertex> vertices = {
        // 前面
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 0
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // 1
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // 2
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},  // 3
        // 后面
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 4
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // 5
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},   // 6
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // 7
        // 顶面
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // 8
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // 9
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // 10
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},  // 11
        // 底面
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // 12
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // 13
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // 14
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}   // 15
    };

    std::vector<unsigned int> indices = {
        // 前面
        0, 1, 2, 2, 3, 0,
        // 右面
        1, 5, 6, 6, 2, 1,
        // 后面
        7, 6, 5, 5, 4, 7,
        // 左面
        4, 0, 3, 3, 7, 4,
        // 顶面
        8, 9, 10, 10, 11, 8,
        // 底面
        12, 13, 14, 14, 15, 12
    };

    Mesh cube(vertices, indices);

    unsigned int texture1;

    Texture::BuildTexture("NagisaKaworu.bmp", texture1);


    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    shader.Use();
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);

    float lastFrame = 0.0f;

    window.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    while (!window.ShouldClose())
    {
        constexpr float rotationSpeed = 50.0f;
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 处理输入
        inputManager.ProcessInput();

        // 相机控制
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);

        // 渲染
        window.Clear();

        // 更新模型矩阵以实现旋转
        glm::mat4 model = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera->GetZoom()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        view = camera->GetViewMatrix();

        shader.Use();
        shader.SetMat4("model", model);
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);

        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // 绘制三角形
        cube.Draw(shader);

        window.SwapBuffersAndPollEvents();
    }

    // 清理
    glDeleteProgram(shader.ID);

    window.Terminate();

    SAFE_DELETE(camera);

    return 0;
}