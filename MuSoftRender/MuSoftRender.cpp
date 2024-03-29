// MuSoftRender.cpp : 定义应用程序的入口点。
//

#include "MuSoftRender.h"
#include "Math/MuMath.h"
#include "Camera/MuCamera.h"
#include "Device/MuDevice.h"
#include "Obj/MuObjModel.h"
#include "Rasterizer/MuRasterizer.h"
#include "framework.h"
#include "Function/MuLog.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                     // 当前实例
WCHAR szTitle[MAX_LOADSTRING];       // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名



// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(
_In_ HINSTANCE hInstance,
_In_opt_ HINSTANCE hPrevInstance,
_In_ LPWSTR lpCmdLine,
_In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MUSOFTRENDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    const HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MUSOFTRENDER));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW Wcex;

    Wcex.cbSize = sizeof(WNDCLASSEX);

    Wcex.style = CS_HREDRAW | CS_VREDRAW;
    Wcex.lpfnWndProc = WndProc;
    Wcex.cbClsExtra = 0;
    Wcex.cbWndExtra = 0;
    Wcex.hInstance = hInstance;
    Wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MUSOFTRENDER));
    Wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    Wcex.hbrBackground = reinterpret_cast<HBRUSH>((COLOR_WINDOW + 1));
    Wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MUSOFTRENDER);
    Wcex.lpszClassName = szWindowClass;
    Wcex.hIconSm = LoadIcon(Wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&Wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    // WNDCLASS wc = {CS_BYTEALIGNCLIENT, (WNDPROC)WndProc, 0, 0, 0,
    //                NULL, NULL, NULL, NULL, _T("SCREEN3.1415926")};
    //
    // wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    // wc.hInstance = GetModuleHandle(NULL);
    // wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    // if (!RegisterClass(&wc)) return -1;
    //
    // const HWND hWnd = CreateWindow(_T("SCREEN3.1415926"), szTitle,
    //                              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    //                              0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, wc.hInstance, nullptr);
    
    const HWND hWnd = CreateWindowW(
        szWindowClass, 
        szTitle, 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 
        0, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        nullptr, 
        nullptr, 
        hInstance, 
        nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // PAINTSTRUCT是Windows API 中的一个结构体，用于保存绘图信息
    PAINTSTRUCT paintStruct;

    // 用于保存绘图设备的句柄
    static HDC hdcBackBuffer;


    static auto* Device = new MuDevice;
    Device->SetRenderMode(EMuRenderMode::Texture);
    static auto Rasterizer = new MuRasterizer;
    static auto* ObjModel = new MuObjModel;
    static auto* Camera = new MuCamera;
    Camera->SetAspectRatio(1.777f);
    Camera->SetFieldOfView(90);
    Camera->SetNearPlane(1.0f);
    Camera->SetFarPlane(2.0f);
    Camera->SetProjectionMode(EProjectionMode::Orthographic);
    Camera->SetCameraPosition(MuPoint4F(0,0,2,0));
    Camera->SetLookAtPoint(MuPoint4F(0,0,0,0));
    Camera->SetUpDirection(MuVector4F(0,1,0,0));
    Camera->Init();
    
    // ModelTransformMatrix 单位矩阵
    // 由于目前直接假定了物体的中心在原点，物体的自身坐标系和世界坐标系重合，所以模型变换矩阵为单位矩阵
    // TODO:设置物体自身的平移旋转和缩放，参考游戏引擎
    static MuMatrix4F ModelTransformMatrix = MuMatrix4F::Identity();
    // 计算ViewTransformMatrix BUG:正交矩阵的计算有问题
    static MuMatrix4F ViewTransformMatrix = MuMatrix4F::Identity();
    // 获取ProjectionTransformMatrix
    static MuMatrix4F ProjectionTransformMatrix = Camera->GetProjectionMatrix();
    // MVP矩阵 右手系
    static MuMatrix4F MVPMatrix = ProjectionTransformMatrix * ViewTransformMatrix * ModelTransformMatrix;

    Camera->SetMVPMatrix(MVPMatrix);
    
    static int clientRectWidth;
    static int clientRectHeight;

    Rasterizer->InitRasterizer(clientRectWidth, clientRectHeight);

    // 用于保存位图句柄
    static HBITMAP hBitmap;

    switch (message)
    {
        case WM_CREATE:
        {
            RECT Rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            AdjustWindowRect(&Rect, GetWindowLong(hWnd, GWL_STYLE), FALSE); 
            GetClientRect(hWnd, &Rect);

            clientRectWidth = Rect.right - Rect.left;
            clientRectHeight = Rect.bottom - Rect.top;
            MuLog::LogInfo( "clientRectWidth: %d, clientRectHeight: %d", clientRectWidth, clientRectHeight);
            SetWindowPos( hWnd, HWND_TOP, 0, 0, clientRectWidth, clientRectHeight, SWP_NOMOVE);
             /*
             * 这段代码定义了一个 BITMAPINFO 结构体变量 Bi，并初始化了它的成员。
             * BITMAPINFO 结构体包含一个 BITMAPINFOHEADER 结构体和一个颜色表。
             * 在这个例子中，颜色表没有被初始化。
             * BITMAPINFOHEADER 结构体包含有关位图的信息，
             * 例如宽度、高度、颜色深度等。
             * 在这个例子中，
             * 位图的宽度和高度分别由 ClientRectWidth 和 ClientRectHeight 变量指定。
             * 颜色深度为 32 位，压缩类型为 BI_RGB（无压缩）。
             * 图像大小由 ClientRectWidth * ClientRectHeight * static_cast<DWORD>(4) 计算得出。
             */
            const BITMAPINFOHEADER bitmapInfoHeader =
                {
                    sizeof(BITMAPINFOHEADER),
                    clientRectWidth,
                    clientRectHeight,
                    1,
                    32,
                    BI_RGB,
                    clientRectWidth * clientRectHeight * static_cast<DWORD>(4),
                    0,
                    0,
                    0,
                    0
                };
            /*
             * RGBQUAD
             * RGBQUAD是Windows API中的一种结构体，
             * 用于表示颜色的四个分量：红色(Red)，绿色(Green)，蓝色(Blue)和Alpha。
             * 其中Alpha表示透明度，取值范围为0~255，0表示完全透明，255表示完全不透明。
             * RGBQUAD通常用于处理图像数据，例如在BMP文件中就使用了RGBQUAD结构体来表示每个像素点的颜色。
                RGBQUAD的定义如下：
                typedef struct tagRGBQUAD {
                    BYTE rgbBlue;
                    BYTE rgbGreen;
                    BYTE rgbRed;
                    BYTE rgbReserved;
                } RGBQUAD;
                其中，rgbBlue、rgbGreen和rgbRed分别表示蓝色、绿色和红色的分量，取值范围也是0~255。rgbReserved用于保留位，通常设置为0。
                在Windows API中，RGBQUAD结构体经常用于处理位图数据（BMP）和调色板（Palette）等图形相关的操作。
                比如，可以使用RGBQUAD结构体来表示一个像素点的颜色信息，或者使用RGBQUAD结构体来表示调色板中每种颜色的信息。
             */
            const RGBQUAD RgbQuad = {};

            /*
             *
             */
            const BITMAPINFO bitmapInfo = {bitmapInfoHeader, RgbQuad};

            /*
             * LPVOID是Windows API中定义的类型。它代表“指向VOID的长指针”，用于表示指向任何类型的指针。Ptr是LPVOID类型的变量，这意味着它可以保存任何类型数据的地址。
             */
            LPVOID Pointer;
            const HDC Hdc = GetDC(hWnd);
            hdcBackBuffer = CreateCompatibleDC(Hdc);
            /*
             * 调用了 CreateDIBSection() 函数来创建一个与设备兼容的位图
             * 第一个是设备上下文句柄（在这里是 HdcBackBuffer）
             * 第二个是指向位图信息结构体的指针（在这里是 &Bitmapinfo）
             * 第三个是颜色表类型（在这里是 DIB_RGB_COLORS）
             * 第四个是指向指针变量的指针（在这里是 &Ptr），它将接收位图数据缓冲区的地址
             * 第五个和第六个参数分别为文件映射对象和文件映射对象中数据的偏移量，在这里都为 0
             */
            hBitmap = CreateDIBSection(hdcBackBuffer, &bitmapInfo, DIB_RGB_COLORS, &Pointer, nullptr, 0);
            
            Device->InitDevice(Pointer, clientRectWidth, clientRectHeight, EMuRenderMode::Texture);
            Rasterizer->InitRasterizer(clientRectWidth, clientRectHeight);

            ObjModel->Load("../african_head.obj");
            auto success = ObjModel->LoadTexture("../african_head_diffuse.tga");
            MuLog:: LogInfo("LoadTexture success: %d", success);
            /*
             * SelectObject(HdcBackBuffer, HBitmap)将位图HBitmap选入后备缓冲区的设备上下文HdcBackBuffer中，以便在该设备上下文中使用该位图。
             */
            SelectObject(hdcBackBuffer, hBitmap);
            ReleaseDC(hWnd, Hdc);
        }
        break;
        case WM_COMMAND:
        {
            const int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
        {
            BeginPaint(hWnd, &paintStruct);
            // 用黑色清除之前的绘制
            BitBlt(hdcBackBuffer, 0, 0, clientRectWidth, clientRectHeight, nullptr, NULL, NULL, BLACKNESS);
            
            {
                // TODO: 在此处添加使用 hdc 的任何绘图代码...
                 Rasterizer->DrawObj(Device, Camera, ObjModel, MuColor::GetRandomMuRGB());
                // 画出摄像机的位置
                Rasterizer->DrawPoint(Device->GetPointBitFrameBuffer(), MuMath::Point4FToPoint2I(Camera->GetCameraPosition()), MuColor::GetRandomMuRGB());
            }

            // 将hdcBackBuffer中的位图绘制到paintStruct.hdc中
            BitBlt(paintStruct.hdc, 0, 0, clientRectWidth, clientRectHeight, hdcBackBuffer, 0, 0, SRCCOPY);
            EndPaint(hWnd, &paintStruct);
        }
        break;
        case WM_SIZE:
        {
            RECT Rect; 
            GetClientRect(hWnd, &Rect);
            int width = Rect.right - Rect.left; 
            int height = Rect.bottom - Rect.top;
            MuLog::LogInfo( "width: %d, height: %d", width, height);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        default:
            break;
    }
    return (INT_PTR)FALSE;
}
