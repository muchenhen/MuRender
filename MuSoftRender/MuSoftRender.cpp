﻿#include <windows.h>
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

// 菜单项ID
constexpr auto ID_FRAMERATE_UNLIMITED = 1000;
constexpr auto ID_FRAMERATE_24 = 1001;
constexpr auto ID_FRAMERATE_30 = 1002;
constexpr auto ID_FRAMERATE_60 = 1003;
constexpr auto ID_FRAMERATE_90 = 1004;
constexpr auto ID_FRAMERATE_120 = 1005;
constexpr auto ID_FRAMERATE_144 = 1006;
constexpr auto ID_FRAMERATE_160 = 1007;
constexpr auto ID_FRAMERATE_240 = 1008;

LARGE_INTEGER G_Frequency;
LARGE_INTEGER G_LastFPSUpdateTime;
int G_FrameCount = 0;
double G_Fps = 0.0;
// 帧率限制
int G_FrameRateLimit = 60; // 0 表示无限制

// 窗口过程函数声明
LRESULT CALLBACK WindowProc(HWND Hwnd, UINT UMsg, WPARAM WParam, LPARAM LParam);

Renderer* G_Renderer = nullptr;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// 渲染函数声明
void Render(HWND Hwnd);
// 更新并绘制 FPS
void UpdateAndDrawFPS(HWND Hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // 注册窗口类
    constexpr wchar_t CLASS_NAME[] = L"Soft Renderer Window Class";

    WNDCLASS Wc = {};
    Wc.lpfnWndProc = WindowProc;
    Wc.hInstance = hInstance;
    Wc.lpszClassName = CLASS_NAME;

    RegisterClass(&Wc);

    // 创建菜单
    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreatePopupMenu();

    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_UNLIMITED, L"Unlimited");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_24, L"24 FPS");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_30, L"30 FPS");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_60, L"60 FPS");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_90, L"90 FPS");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_120, L"120 FPS");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_144, L"144 FPS");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_160, L"160 FPS");
    AppendMenu(hSubMenu, MF_STRING, ID_FRAMERATE_240, L"240 FPS");

    AppendMenu(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu), L"Frame Rate");

    RECT rc = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, true);

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

    // 设置菜单
    SetMenu(Hwnd, hMenu);

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
    QueryPerformanceCounter(&G_LastFPSUpdateTime);

    // 创建Render实例
    G_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

    ShowWindow(Hwnd, nCmdShow);

    // 消息循环
    MSG Msg = {};
    LARGE_INTEGER lastFrameTime;
    QueryPerformanceCounter(&lastFrameTime);

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
            LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);

            double deltaTime = (currentTime.QuadPart - lastFrameTime.QuadPart) / static_cast<double>(G_Frequency.QuadPart);

            if (G_FrameRateLimit == 0 || deltaTime >= 1.0 / G_FrameRateLimit)
            {
                // 更新 FPS
                G_FrameCount++;
                double timeSinceLastFPSUpdate = (currentTime.QuadPart - G_LastFPSUpdateTime.QuadPart) / static_cast<double>(G_Frequency.QuadPart);

                if (timeSinceLastFPSUpdate >= 1.0) // 每秒更新一次 FPS
                {
                    G_Fps = G_FrameCount / timeSinceLastFPSUpdate;
                    G_FrameCount = 0;
                    G_LastFPSUpdateTime = currentTime;
                }

                Render(Hwnd);
                UpdateAndDrawFPS(Hwnd);
                lastFrameTime = currentTime;
            }
            else
            {
                // 使用更精确的等待方法
                LARGE_INTEGER targetTime;
                targetTime.QuadPart = lastFrameTime.QuadPart + (G_Frequency.QuadPart / G_FrameRateLimit);
                while (currentTime.QuadPart < targetTime.QuadPart)
                {
                    QueryPerformanceCounter(&currentTime);
                }
            }
        }
    }

    SAFE_DELETE(G_Renderer);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND Hwnd, UINT UMsg, WPARAM WParam, LPARAM LParam)
{
    switch (UMsg)
    {
        case WM_COMMAND:
        {
            switch (LOWORD(WParam))
            {
                case ID_FRAMERATE_UNLIMITED:
                    G_FrameRateLimit = 0;
                    break;
                case ID_FRAMERATE_24:
                    G_FrameRateLimit = 24;
                    break;
                case ID_FRAMERATE_30:
                    G_FrameRateLimit = 30;
                    break;
                case ID_FRAMERATE_60:
                    G_FrameRateLimit = 60;
                    break;
                case ID_FRAMERATE_90:
                    G_FrameRateLimit = 90;
                    break;
                case ID_FRAMERATE_120:
                    G_FrameRateLimit = 120;
                    break;
                case ID_FRAMERATE_144:
                    G_FrameRateLimit = 144;
                    break;
                case ID_FRAMERATE_160:
                    G_FrameRateLimit = 160;
                    break;
                case ID_FRAMERATE_240:
                    G_FrameRateLimit = 240;
                    break;
            }
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

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
            return 0;
        }
        default:;
    }
    return DefWindowProc(Hwnd, UMsg, WParam, LParam);
}

void Render(HWND Hwnd)
{
    G_Renderer->Clear(0x000000); // 清除为黑色

    G_Renderer->DrawLine(0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, 0xFFFFFF); // 白线

    // 创建位图
    BITMAPINFO Bmi = {};
    Bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Bmi.bmiHeader.biWidth = G_Renderer->GetWidth();
    Bmi.bmiHeader.biHeight = -G_Renderer->GetHeight(); // 负值表示从上到下的位图
    Bmi.bmiHeader.biPlanes = 1;
    Bmi.bmiHeader.biBitCount = 32;
    Bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(Hwnd);
    SetDIBitsToDevice(hdc, 0, 0, G_Renderer->GetWidth(), G_Renderer->GetHeight(), 0, 0, 0, G_Renderer->GetHeight(),
                      G_Renderer->GetFrameBuffer().data(), &Bmi, DIB_RGB_COLORS);
    ReleaseDC(Hwnd, hdc);
}

void UpdateAndDrawFPS(HWND Hwnd)
{
    static std::string lastFpsText;
    static LARGE_INTEGER lastUpdateTime = {0};

    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    // 每 500ms 更新一次 FPS 显示
    if ((currentTime.QuadPart - lastUpdateTime.QuadPart) / static_cast<double>(G_Frequency.QuadPart) >= 0.5)
    {
        std::ostringstream Oss;
        Oss << "FPS: " << std::fixed << std::setprecision(1) << G_Fps;
        if (G_FrameRateLimit > 0)
        {
            Oss << " (Limit: " << G_FrameRateLimit << ")";
        }
        else
        {
            Oss << " (Unlimited)";
        }
        lastFpsText = Oss.str();
        lastUpdateTime = currentTime;
    }

    // 绘制 FPS 文本
    HDC Hdc = GetDC(Hwnd);
    SetBkMode(Hdc, TRANSPARENT);
    SetTextColor(Hdc, RGB(255, 255, 255));
    TextOutA(Hdc, 10, 10, lastFpsText.c_str(), lastFpsText.length());
    ReleaseDC(Hwnd, Hdc);
}