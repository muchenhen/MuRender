#include <memory>
#include "Window.h"
#include "Camera.h"
#include "InputManager.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

class Application
{
private:
    Window m_window;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Camera> m_camera;
    InputManager& m_inputManager;
    Scene m_scene;
    std::shared_ptr<Texture> m_texture;

public:
    Application();

    void Run();

private:
    void Initialize();

    void LoadResources();

    void MainLoop();

    void Cleanup();
};