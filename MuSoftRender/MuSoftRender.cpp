#include <windows.h>
#include <vector>
#include <cstdint>

#include "Render.h"

#define SAFE_DELETE(p)     \
    {                      \
        if (p)             \
        {                  \
            delete (p);    \
            (p) = nullptr; \
        }                  \
    }

// 窗口过程函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Renderer* G_Renderer = nullptr;

// 渲染函数声明
void Render();

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // 注册窗口类
    constexpr wchar_t CLASS_NAME[] = L"Soft Renderer Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // 创建窗口
    const HWND hwnd = CreateWindowEx(
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

    if (hwnd == nullptr)
    {
        return 0;
    }

    // 创建Render实例
    G_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

    ShowWindow(hwnd, nCmdShow);

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    SAFE_DELETE(G_Renderer);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            const HDC hdc = BeginPaint(hwnd, &ps);

            Render();

            // 创建位图
            BITMAPINFO bmi = {};
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = G_Renderer->GetWidth();
            bmi.bmiHeader.biHeight = -G_Renderer->GetHeight(); // 负值表示从上到下的位图
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            // 将缓冲区内容绘制到窗口
            SetDIBitsToDevice(hdc, 0, 0, G_Renderer->GetWidth(), G_Renderer->GetHeight(), 0, 0, 0, G_Renderer->GetHeight(),
                              G_Renderer->GetFrameBuffer().data(), &bmi, DIB_RGB_COLORS);

            EndPaint(hwnd, &ps);
        }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 简单的渲染函数
void Render()
{
    G_Renderer->Clear(0);

    // 画线
    G_Renderer->DrawLine(100, 100, 700, 500, 0xFFFF0000);
    G_Renderer->DrawLine(100, 500, 700, 100, 0xFFFF0000);

    // 画三角形
    G_Renderer->DrawTriangle(100, 100, 700, 100, 400, 500, 0xFF00FF00);
}