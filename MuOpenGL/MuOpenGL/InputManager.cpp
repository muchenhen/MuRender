﻿#include "InputManager.h"

void InputManager::Initialize(GLFWwindow* windows)
{
    m_window = windows;
    glfwSetCursorPosCallback(m_window, [](GLFWwindow*, double xpos, double ypos)
    {
        GetInstance().MouseCallback(xpos, ypos);
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow*, double, double yoffset)
    {
        GetInstance().ScrollCallback(yoffset);
    });
}

void InputManager::ProcessInput(double deltaTime)
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) m_activeCamera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) m_activeCamera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) m_activeCamera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) m_activeCamera->ProcessKeyboard(RIGHT, deltaTime);
}

void InputManager::MouseCallback(double xpos, double ypos)
{
    if (m_firstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos;

    m_lastX = xpos;
    m_lastY = ypos;

    m_activeCamera->ProcessMouseMovement(xoffset, yoffset);
}

void InputManager::ScrollCallback(double yoffset)
{
    m_activeCamera->ProcessMouseScroll(yoffset);
}

void InputManager::SetCamera(Camera* camera)
{
    m_activeCamera = camera;
}

InputManager::InputManager()
{
}