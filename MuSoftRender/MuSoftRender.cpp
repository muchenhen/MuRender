#include <windows.h>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "Render.h"

#define SAFE_DELETE(p)     \
    {                      \
        if (p)             \
        {                  \
            delete (p);    \
            (p) = nullptr; \
        }                  \
    }

LARGE_INTEGER G_Frequency;
LARGE_INTEGER G_LastCounter;
double G_ElapsedTime = 0.0;
int G_FrameCount = 0;
double G_Fps = 0.0;

// 窗口过程函数声明
LRESULT CALLBACK WindowProc(HWND Hwnd, UINT UMsg, WPARAM WParam, LPARAM LParam);

Renderer* G_Renderer = nullptr;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// 渲染函数声明
void Render(HWND Hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // 注册窗口类
    constexpr wchar_t CLASS_NAME[] = L"Soft Renderer Window Class";

    WNDCLASS Wc = {};
    Wc.lpfnWndProc = WindowProc;
    Wc.hInstance = hInstance;
    Wc.lpszClassName = CLASS_NAME;

    RegisterClass(&Wc);

    RECT rc = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // 创建窗口
    const HWND Hwnd = CreateWindowEx(
        0,                            // 扩展窗口样式
        CLASS_NAME,                   // 窗口类名
        L"Soft Renderer",             // 窗口标题
        WS_OVERLAPPEDWINDOW,          // 窗口样式
        CW_USEDEFAULT, CW_USEDEFAULT, // 位置
        WINDOW_WIDTH, WINDOW_HEIGHT,  // 大小
        nullptr,                      // 父窗口
        nullptr,                      // 菜单
        hInstance,                    // 实例句柄
        nullptr                       // 额外数据
    );

    if (Hwnd == nullptr)
    {
        return 0;
    }

    // 验证客户区大小
    RECT clientRect;
    GetClientRect(Hwnd, &clientRect);
    if (clientRect.right != WINDOW_WIDTH || clientRect.bottom != WINDOW_HEIGHT)
    {
        // 如果大小不匹配，进行调整
        RECT windowRect;
        GetWindowRect(Hwnd, &windowRect);
        int width = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + WINDOW_WIDTH;
        int height = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + WINDOW_HEIGHT;
        SetWindowPos(Hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
    }

    QueryPerformanceFrequency(&G_Frequency);
    QueryPerformanceCounter(&G_LastCounter);

    // 创建Render实例
    G_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

    ShowWindow(Hwnd, nCmdShow);

    // 消息循环
    MSG Msg = {};
    while (true)
    {
        if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (Msg.message == WM_QUIT)
                break;

            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        else
        {
            Render(Hwnd);
        }
    }

    SAFE_DELETE(G_Renderer);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND Hwnd, UINT UMsg, WPARAM WParam, LPARAM LParam)
{
    switch (UMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT Ps;
            HDC Hdc = BeginPaint(Hwnd, &Ps);

            Render(Hwnd);

            // 创建位图
            BITMAPINFO Bmi = {};
            Bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            Bmi.bmiHeader.biWidth = G_Renderer->GetWidth();
            Bmi.bmiHeader.biHeight = -G_Renderer->GetHeight(); // 负值表示从上到下的位图
            Bmi.bmiHeader.biPlanes = 1;
            Bmi.bmiHeader.biBitCount = 32;
            Bmi.bmiHeader.biCompression = BI_RGB;

            SetDIBitsToDevice(Hdc, 0, 0, G_Renderer->GetWidth(), G_Renderer->GetHeight(), 0, 0, 0, G_Renderer->GetHeight(),
                              G_Renderer->GetFrameBuffer().data(), &Bmi, DIB_RGB_COLORS);

            EndPaint(Hwnd, &Ps);
        }
            return 0;
    }
    return DefWindowProc(Hwnd, UMsg, WParam, LParam);
}

// 简单的渲染函数
void Render(HWND Hwnd)
{
    G_Renderer->Clear(0x000000); // 清除为黑色

    G_Renderer->DrawLine(0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, 0xFFFFFF); // 白线
    // G_Renderer->FillTriangle(150, 50, 250, 200, 50, 200, 0xFF0000); // 红色三角形

    LARGE_INTEGER CurrentCounter;
    QueryPerformanceCounter(&CurrentCounter);
    double DeltaTime = (CurrentCounter.QuadPart - G_LastCounter.QuadPart) / (double)G_Frequency.QuadPart;
    G_LastCounter = CurrentCounter;

    G_ElapsedTime += DeltaTime;
    G_FrameCount++;

    if (G_ElapsedTime >= 1.0)
    {
        G_Fps = G_FrameCount / G_ElapsedTime;
        G_ElapsedTime = 0.0;
        G_FrameCount = 0;
    }

    HDC Hdc = GetDC(Hwnd);
    SetBkMode(Hdc, TRANSPARENT);
    SetTextColor(Hdc, RGB(255, 255, 255));

    std::ostringstream Oss;
    Oss << "FPS: " << std::fixed << std::setprecision(1) << G_Fps;
    std::string FpsText = Oss.str();

    TextOutA(Hdc, 10, 10, FpsText.c_str(), FpsText.length());

    ReleaseDC(Hwnd, Hdc);

    InvalidateRect(Hwnd, nullptr, FALSE);
}