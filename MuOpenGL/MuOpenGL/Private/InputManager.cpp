#include "InputManager.h"

InputManager::InputManager() = default;

InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::Initialize(GLFWwindow* windows)
{
    m_window = windows;
    glfwSetCursorPosCallback(m_window, [](GLFWwindow*, double xPos, double yPos)
    {
        GetInstance().MouseCallback(xPos, yPos);
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow*, double, double yOffset)
    {
        GetInstance().ScrollCallback(yOffset);
    });
}

void InputManager::ProcessInput(double deltaTime) const
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, true);
    }
    // 处理左右方向键
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        m_activeCamera->OrbitAroundTarget(m_orbitSpeed * static_cast<float>(deltaTime));
    }
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        m_activeCamera->OrbitAroundTarget(-m_orbitSpeed * static_cast<float>(deltaTime));
    }
}

void InputManager::MouseCallback(double xPos, double yPos)
{
    if (m_firstMouse)
    {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouse = false;
    }

    const float xOffset = static_cast<float>(xPos - m_lastX);
    const float yOffset = static_cast<float>(m_lastY - yPos);

    m_lastX = xPos;
    m_lastY = yPos;

    m_activeCamera->ProcessMouseMovement(xOffset, yOffset);
}

void InputManager::ScrollCallback(double yOffset) const
{
}

void InputManager::SetCamera(Camera* camera)
{
    m_activeCamera = camera;
}