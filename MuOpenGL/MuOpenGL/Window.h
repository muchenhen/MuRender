#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    GLFWwindow* GetWindow();
    
    void Clear();
    void SetClearColor(float r, float g, float b, float a);
    bool ShouldClose();
    void ProcessInput();
    void SwapBuffersAndPollEvents();
    void Terminate();

    void SetMouseCallback(GLFWcursorposfun callback);
    void SetScrollCallback(GLFWscrollfun callback);
    
private:
    GLFWwindow* m_window;

    float m_clearColor[4];
};