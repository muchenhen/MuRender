#pragma once
#include "Camera.h"
#include <GLFW/glfw3.h>


class InputManager
{
private:
    InputManager();
    ~InputManager() = default;

    GLFWwindow* m_window;
    bool m_firstMouse;
    double m_lastX, m_lastY;
    Camera* m_activeCamera;

public:
    static InputManager& GetInstance();

public:
    void Initialize(GLFWwindow* windows);
    void ProcessInput(double deltaTime) const;
    void MouseCallback(double xPos, double yPos);
    void ScrollCallback(double yOffset) const;

    void SetCamera(Camera* camera);
};