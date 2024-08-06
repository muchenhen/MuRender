#include "Logger.h"
#include "Camera.h"
#include "Cube.h"
#include "Render.h"
#include "Scene.h"
#include "Functions.h"

#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>


#include <windows.h>

#include "DirectionalLight.h"
#include "Floor.h"
#include "CoordinateSystem .h"
#include "Sphere.h"


#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

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

RenderPipeline* G_RenderPipeline = nullptr;
NormalRenderPipeline* G_NormalRenderPipeline = nullptr;
Renderer* G_Renderer = nullptr;
Scene* G_Scene = nullptr;
Camera* G_Camera = nullptr;
DirectionalLight* G_DirectionalLight = nullptr;

// 鼠标控制相关
bool g_IsMouseDown = false;
int g_LastMouseX = 0;
int g_LastMouseY = 0;
float g_CameraDistance = 5.0f; // 初始摄像机距离

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr float UNITS_PER_METER = 1.0f;

void Tick(HWND Hwnd, float deltaTime);
// 渲染函数声明
void Render(HWND Hwnd);
// 更新并绘制 FPS
void UpdateAndDrawFPS(HWND Hwnd);
void UpdateDevice(HWND Hwnd);

// 注意：本项目采用1单位 = 1米的比例，与UE4保持一致

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    Logger::GetInstance().SetLogLevel(Logger::ELogLevel::Debug);
    Logger::GetInstance().SetLogFile("debug_log.txt");

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
        SetCursorPos((windowRect.right + windowRect.left) / 2, (windowRect.bottom + windowRect.top) / 2);
    }

    QueryPerformanceFrequency(&G_Frequency);
    QueryPerformanceCounter(&G_LastFPSUpdateTime);

    // 创建RenderPipeline实例
    G_RenderPipeline = new RenderPipeline(DefaultStandardVertexShader, DefaultFragmentShader);
    G_NormalRenderPipeline = new NormalRenderPipeline(DefaultNormalVertexShader, DefaultSimpleLitFragmentShader);
    // 创建Render实例
    G_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
    // 创建Scene实例
    G_Scene = new Scene();

    // 创建Camera实例
    Eigen::Vector3f CameraPosition = Eigen::Vector3f(8, 8, -8);
    std::unique_ptr<Camera> CameraPtr = std::make_unique<Camera>(
        CameraPosition,
        Eigen::Vector3f(0, 0, 0),
        Eigen::Vector3f(0, 1, 0),
        45.0f,
        static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT,
        0.1f,
        100.0f
        );

    G_Scene->AddCamera(std::move(CameraPtr));

    G_Camera = G_Scene->GetCameras()[0].get();

    std::shared_ptr<Texture> TexturePtr = std::make_shared<Texture>();
    TexturePtr->LoadFromFile("..\\Resource\\NagisaKaworu.bmp");

    std::shared_ptr<Material> MaterialPtr = std::make_shared<Material>();
    MaterialPtr->SetTexture(TexturePtr);

    std::shared_ptr<Cube> CubePtr = std::make_shared<Cube>(2.0f);
    CubePtr->SetMaterial(MaterialPtr);

    std::shared_ptr<Sphere> SpherePtr = std::make_shared<Sphere>(0.5f, 8, 4);
    SpherePtr->SetMaterial(MaterialPtr);

    std::shared_ptr<Floor> FloorPtr = std::make_shared<Floor>();
    float PosY = -1;
    FloorPtr->SetPosition(Eigen::Vector3f(0, PosY, 0));
    FloorPtr->SetScale(Eigen::Vector3f(1, 1, 1));

    std::shared_ptr<Material> FloorMaterialPtr = std::make_shared<Material>();
    FloorMaterialPtr->SetBaseColor(Eigen::Vector3f(0.67f, 0.67f, 0.67f));
    FloorPtr->SetMaterial(FloorMaterialPtr);
    FloorPtr->SetCastShadow(false);

    V3f LightDir = V3f(0, 1, 0).normalized();

    std::shared_ptr<DirectionalLight> LightPtr = std::make_shared<DirectionalLight>(
        LightDir,
        Eigen::Vector3f(1, 1, 1),
        1.0f
        );

    G_DirectionalLight = LightPtr.get();

    // std::shared_ptr<CoordinateSystem> CoordinateSystemPtr = std::make_shared<CoordinateSystem>(2.5f, 0.3f);
    // G_Scene->AddObject(CoordinateSystemPtr);

    G_Scene->AddObject(CubePtr);
    G_Scene->AddObject(FloorPtr);

    G_Scene->SetDirectionalLight(LightPtr);

    ShowWindow(Hwnd, nCmdShow);

    // 消息循环
    MSG Msg = {};
    LARGE_INTEGER lastFrameTime;
    QueryPerformanceCounter(&lastFrameTime);

    while (true)
    {
        if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (Msg.message == WM_QUIT) break;

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

                Tick(Hwnd, static_cast<float>(deltaTime));

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

    SAFE_DELETE(G_Renderer)
    SAFE_DELETE(G_Scene)
    SAFE_DELETE(G_Camera)
    return 0;
}

LRESULT CALLBACK WindowProc(HWND Hwnd, UINT UMsg, WPARAM WParam, LPARAM LParam)
{
    switch (UMsg)
    {
        case WM_LBUTTONDOWN:
        {
            g_IsMouseDown = true;
            g_LastMouseX = GET_X_LPARAM(LParam);
            g_LastMouseY = GET_Y_LPARAM(LParam);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            g_IsMouseDown = false;
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            if (g_IsMouseDown)
            {
                int x = GET_X_LPARAM(LParam);
                int y = GET_Y_LPARAM(LParam);
                int dx = x - g_LastMouseX;
                int dy = y - g_LastMouseY;

                // 判断Object是否存在
                if (!G_Scene->GetObjects().empty())
                {
                    // 更新 Cube 的旋转
                    Eigen::Vector3f targetRotation = G_Scene->GetObjects()[0]->GetRotation() + Eigen::Vector3f(dy * 0.01f, dx * 0.01f, 0.0f);
                    Eigen::Vector3f currentRotation = G_Scene->GetObjects()[0]->GetRotation();

                    Eigen::Vector3f newRotation = Lerp(currentRotation, targetRotation, 0.1f);
                    G_Scene->GetObjects()[0]->SetRotation(newRotation);
                    G_Scene->GetObjects()[0]->SetRotation(G_Scene->GetObjects()[0]->GetRotation() + Eigen::Vector3f(dy * 0.01f, dx * 0.01f, 0.0f));
                }

                g_LastMouseX = x;
                g_LastMouseY = y;
            }
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            return 0;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(WParam))
            {
                case ID_FRAMERATE_UNLIMITED: G_FrameRateLimit = 0;
                    break;
                case ID_FRAMERATE_24: G_FrameRateLimit = 24;
                    break;
                case ID_FRAMERATE_30: G_FrameRateLimit = 30;
                    break;
                case ID_FRAMERATE_60: G_FrameRateLimit = 60;
                    break;
                case ID_FRAMERATE_90: G_FrameRateLimit = 90;
                    break;
                case ID_FRAMERATE_120: G_FrameRateLimit = 120;
                    break;
                case ID_FRAMERATE_144: G_FrameRateLimit = 144;
                    break;
                case ID_FRAMERATE_160: G_FrameRateLimit = 160;
                    break;
                case ID_FRAMERATE_240: G_FrameRateLimit = 240;
                    break;
            }
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (!G_Camera)
            {
                return 0;
            }

            float MoveSpeed = 0.1f;
            Eigen::Vector3f Forward = G_Camera->GetForward();
            Eigen::Vector3f Right = G_Camera->GetRight();
            Eigen::Vector3f Up = G_Camera->GetUp();
            Eigen::Vector3f Position = G_Camera->GetPosition();

            switch (WParam)
            {
                case 'W':
                {
                    Position += Forward * MoveSpeed;
                    break;
                }
                case 'S':
                {
                    Position -= Forward * MoveSpeed;
                    break;
                }
                case 'A':
                {
                    Position -= Right * MoveSpeed;
                    break;
                }
                case 'D':
                {
                    Position += Right * MoveSpeed;
                    break;
                }
                case 'Q':
                {
                    Position -= Up * MoveSpeed;
                    break;
                }
                case 'E':
                {
                    Position += Up * MoveSpeed;
                    break;
                }
            }

            G_Camera->SetPosition(Position);
            // G_DirectionalLight->Direction = - Position.normalized();
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT Ps;
            HDC Hdc = BeginPaint(Hwnd, &Ps);

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
        default: ;
    }
    return DefWindowProc(Hwnd, UMsg, WParam, LParam);
}

void Tick(HWND Hwnd, float deltaTime)
{
    G_Scene->Update(deltaTime);

    Render(Hwnd);
}

bool rendered = false;

void Render(HWND Hwnd)
{
    G_Renderer->Clear(0x000000); // 清除为黑色

    if (G_Renderer && G_Scene && !G_Scene->GetCameras().empty())
    {
        // G_Renderer->RenderCamera(*G_Scene, *G_Scene->GetCameras()[0]);
        // G_Renderer->RenderScene(G_Scene, G_Scene->GetCameras()[0].get(), G_RenderPipeline);
        G_Renderer->RenderScene(G_Scene, G_Scene->GetCameras()[0].get(), G_NormalRenderPipeline);
    }

    UpdateDevice(Hwnd);
}

void UpdateDevice(HWND Hwnd)
{
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
    TextOutA(Hdc, 10, 10, lastFpsText.c_str(), static_cast<int>(lastFpsText.length()));
    ReleaseDC(Hwnd, Hdc);
}