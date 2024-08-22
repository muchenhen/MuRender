#include "Constants.h"

#include "Application.h"

Application::Application() :
    m_window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL"),
    m_inputManager(InputManager::GetInstance())
{
}

void Application::Run()
{
    Initialize();
    LoadResources();
    MainLoop();
    Cleanup();
}

void Application::Initialize()
{
    m_inputManager.Initialize(m_window.GetWindow());
    m_window.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    m_camera = std::make_shared<Camera>();
    m_camera->SetPosition(glm::vec3(3.0f, 3.0f, 3.0f));
    m_camera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->SetFOV(90.0f);
    m_camera->SetAspectRatio(static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT));
    m_camera->SetName("Camera");

    m_inputManager.SetCamera(m_camera.get());

    m_shader = std::make_shared<Shader>(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());

    m_scene.SetCamera(m_camera);
    m_scene.SetShader(m_shader);
}

void Application::LoadResources()
{
    auto cube = std::shared_ptr<MeshObject>(MeshObject::CreateCube("Cube"));
    cube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_scene.AddObject(cube);

    // m_texture = std::make_shared<Texture>();
    // if (m_texture->Load("NagisaKaworu.bmp"))
    // {
    //     // 纹理加载成功
    //     cube->SetTexture(m_texture);
    // }
}

void Application::MainLoop()
{
    double lastFrame = 0.0f;

    while (!m_window.ShouldClose())
    {
        double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_inputManager.ProcessInput(deltaTime);
        m_scene.Update(deltaTime);

        m_window.Clear();
        m_scene.Render();
        m_window.SwapBuffersAndPollEvents();
    }
}

void Application::Cleanup()
{
    m_window.Terminate();
}