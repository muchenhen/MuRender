#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();
    void Clear();
    void SetClearColor(float r, float g, float b, float a);
    bool ShouldClose();
    void ProcessInput();
    void SwapBuffersAndPollEvents();
    void Terminate();

private:
    GLFWwindow* m_window;

    float m_clearColor[4];
};