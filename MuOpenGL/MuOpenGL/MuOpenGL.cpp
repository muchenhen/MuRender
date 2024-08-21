#include "Window.h"

#include "Constants.h"
#include "MeshObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "InputManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }

int main(int argc, char* argv[])
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");

    Shader shader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());

    Camera* camera = new Camera();
    camera->SetPosition(glm::vec3(3.0f, 3.0f, 3.0f));
    camera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    camera->SetFOV(90.0f);
    camera->SetName("Camera");

    InputManager& inputManager = InputManager::GetInstance();
    inputManager.Initialize(window.GetWindow());
    inputManager.SetCamera(camera);

    MeshObject* cube = MeshObject::CreateCube("Cube");
    cube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    unsigned int texture1;
    Texture::BuildTexture("NagisaKaworu.bmp", texture1);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    shader.Use();
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);

    double lastFrame = 0.0f;

    window.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!window.ShouldClose())
    {
        double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 处理输入
        inputManager.ProcessInput(deltaTime);

        // 相机控制

        // 渲染
        window.Clear();

        // 更新模型矩阵以实现旋转
        glm::mat4 model = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera->GetFOV()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        view = camera->GetViewMatrix();

        shader.Use();
        shader.SetMat4("model", model);
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);

        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // 绘制三角形
        cube->Draw(shader);

        window.SwapBuffersAndPollEvents();
    }

    // 清理
    glDeleteProgram(shader.ID);

    window.Terminate();

    SAFE_DELETE(camera)

    return 0;
}