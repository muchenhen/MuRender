#include "window.h"
#include <iostream>
// 回调函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window(int width, int height, const char* title) :
    m_window(nullptr)
{
    // 初始化GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';
        return;
    }

    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return;
    }

    // 设置当前上下文
    glfwMakeContextCurrent(m_window);

    // 设置帧缓冲区大小回调
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << '\n';
        return;
    }
}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Window::Clear()
{
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SetClearColor(float r, float g, float b, float a)
{
    m_clearColor[0] = r;
    m_clearColor[1] = g;
    m_clearColor[2] = b;
    m_clearColor[3] = a;
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void Window::ProcessInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void Window::SwapBuffersAndPollEvents()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::Terminate()
{
    glfwTerminate();
}

// 帧缓冲区大小回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}