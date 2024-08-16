#pragma once
#include "Camera.h"
#include <GLFW/glfw3.h>


class InputManager
{
public:
    static InputManager& GetInstance()
    {
        static InputManager instance;
        return instance;
    }

    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&) = delete;

public:
    void Initialize(GLFWwindow* windows);
    void ProcessInput();
    void MouseCallback(double xpos, double ypos);
    void ScrollCallback(double yoffset);
    
    void SetCamera(Camera* camera);

private:
    InputManager();

    GLFWwindow* m_window;
    bool m_firstMouse;
    float m_lastX, m_lastY;
    Camera* m_activeCamera;
};