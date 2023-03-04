#include <iostream>
#include <windowsx.h>
#include <ctime>
#include <string>
#include "Device.h"
#include "MathUnion.h"
using namespace std;

//  这里有规范建议使用枚举类型来定义整数常量，而不是使用宏定义
// 但是此处并不是有连续含义或者有关联的整数常量，所以使用宏定义更加合适
// 这里定义了一个鼠标滚轮消息的宏
#define WM_MOUSEWHEEL 0x020A

// 定义屏幕长宽
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

// 窗口标题
string WindowTitle = "MuRender";

unsigned int gModelIndex = 1;
unique_ptr<Device> gDevice = make_unique<Device>();

// Convert the string to an LPCWSTR
LPCWSTR StringToLPCWSTR(const std::string& Orig)
{
    // Get the length of the string in bytes
    const size_t SizeInWords = Orig.length() + 1;

    // Create a buffer to hold the data
    size_t ConvertedChars = 0;
    const auto Wcstring = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (Orig.length() - 1)));

    // Convert the string
    mbstowcs_s(&ConvertedChars, Wcstring, SizeInWords, Orig.c_str(), _TRUNCATE);

    // Return the converted string
    return Wcstring;
}

// Description:
// This function is used to output text to screen. The text is written to the
// screen at the location specified by the X and Y parameters.
//
// Parameters:
// HDC		- Handle to the device context.
// X		- The X coordinate of the output location.
// Y		- The Y coordinate of the output location.
// Str		- The string to output.
void TextOutX(const HDC HDC, const int X, const int Y, const std::string& Str)
{
    const LPCWSTR Pstr = StringToLPCWSTR(Str);
    TextOut(HDC, X, Y, Pstr, Str.size());
}

LRESULT CALLBACK OnEvent(HWND, unsigned int, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    // Define the window class
    WNDCLASS Wndclass;
    // Set the style for the window class
    Wndclass.style = CS_HREDRAW | CS_VREDRAW;
    // Set the event handler for the window class
    Wndclass.lpfnWndProc = static_cast<WNDPROC>(OnEvent);
    // Set the amount of extra memory to allocate for this window class
    Wndclass.cbClsExtra = 0;
    // Set the amount of extra memory to allocate for each window created with this window class
    Wndclass.cbWndExtra = 0;
    // Set the handle to the instance of the application
    Wndclass.hInstance = GetModuleHandle(nullptr);
    // Set the handle to the icon to use for this window
    Wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    // Set the handle to the cursor to use for this window
    Wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    // Set the handle to the brush to use for the background for this window
    Wndclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    // Set the menu name
    Wndclass.lpszMenuName = nullptr;
    // Set the class name
    Wndclass.lpszClassName = TEXT("MainWnd");

    if (!RegisterClass(&Wndclass))
    {
        MessageBox(nullptr, TEXT("RegisterClass Failed."), nullptr, 0);
        return false;
    }

    RECT Rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);
    const int Width = Rect.right - Rect.left;
    const int Height = Rect.bottom - Rect.top;

    const HWND GHWND = CreateWindow(TEXT("MainWnd"), TEXT("MuRender"),
                                    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                    Width, Height, NULL, NULL, Wndclass.hInstance, NULL);
    if (!GHWND)
    {
        MessageBox(nullptr, TEXT("CreateWindow Failed."), nullptr, 0);
        return false;
    }

    ShowWindow(GHWND, SW_SHOW);
    UpdateWindow(GHWND);

    MSG Msg = {nullptr};
    while (GetMessage(&Msg, nullptr, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return static_cast<int>(Msg.wParam);
}

LRESULT CALLBACK OnEvent(const HWND Hwnd, const UINT Message, const WPARAM WParam, const LPARAM LParam)
{
    PAINTSTRUCT PaintStruct;
    HDC Hdc;

    static int ClientRectWidth, ClientRectHeight;
    static HDC HdcBackBuffer;
    static HBITMAP HBitmap;
    static HBITMAP HOldBitmap;

    switch (Message)
    {
    case WM_CREATE: {
        // 这段代码定义了一个 RECT 结构体变量 Rect，并使用 GetClientRect 函数获取窗口的客户区域。
        // GetClientRect 函数接受两个参数：一个窗口句柄和一个指向 RECT 结构体的指针。
        // 函数将客户区域的坐标存储在 RECT 结构体中。
        RECT Rect; // RECT 是一个结构体，用于定义矩形的左上角和右下角的坐标。它包含四个 LONG 类型的成员：left, top, right, 和 bottom。这些成员分别表示矩形的左边界、上边界、右边界和下边界。
        GetClientRect(Hwnd, &Rect);

        ClientRectWidth = Rect.right - Rect.left;
        ClientRectHeight = Rect.bottom - Rect.top;

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
        const BITMAPINFOHEADER Bitmapinfoheader =
            {
                sizeof(BITMAPINFOHEADER),
                ClientRectWidth,
                ClientRectHeight,
                1,
                32,
                BI_RGB,
                ClientRectWidth * ClientRectHeight * static_cast<DWORD>(4),
                0,
                0,
                0,
                0
            };
        const RGBQUAD RGBQuad = {};
        const BITMAPINFO Bitmapinfo = {Bitmapinfoheader, RGBQuad};

        /*
         * LPVOID是Windows API中定义的类型。它代表“指向VOID的长指针”，用于表示指向任何类型的指针。Ptr是LPVOID类型的变量，这意味着它可以保存任何类型数据的地址。
         */
        LPVOID Ptr;
        /*
         * Hdc 是一个设备上下文句柄，它通过调用 GetDC(Hwnd) 来获取，其中 Hwnd 是窗口句柄
         */
        Hdc = GetDC(Hwnd);
        // HdcBackBuffer 是一个与 Hdc 兼容的内存设备上下文，它通过调用 CreateCompatibleDC(Hdc) 来创建。
        HdcBackBuffer = CreateCompatibleDC(Hdc);
        /*
         * 调用了 CreateDIBSection() 函数来创建一个与设备兼容的位图
         * 第一个是设备上下文句柄（在这里是 HdcBackBuffer）
         * 第二个是指向位图信息结构体的指针（在这里是 &Bitmapinfo）
         * 第三个是颜色表类型（在这里是 DIB_RGB_COLORS）
         * 第四个是指向指针变量的指针（在这里是 &Ptr），它将接收位图数据缓冲区的地址
         * 第五个和第六个参数分别为文件映射对象和文件映射对象中数据的偏移量，在这里都为 0
         */
        HBitmap = CreateDIBSection(HdcBackBuffer, &Bitmapinfo, DIB_RGB_COLORS, &Ptr, nullptr, 0);

        if (!HBitmap)
        {
            MessageBox(nullptr, TEXT("create dib section failed!"), TEXT("error"), MB_OK);
            return 0;
        }

        gDevice->Initialize(ClientRectWidth, ClientRectHeight, Ptr);

        Ptr = nullptr;
        gDevice->pModel->CreateBox3D();
        gDevice->pModel->CreateTriangle3D();
        gDevice->pModel->ObjBuffer("./Model/diablo3_pose.obj");
        gDevice->pTexture->LoadBMP("./Texture/Res/diablo3_pose_diffuse.bmp");
        // SelectObject(HdcBackBuffer, HBitmap)将位图HBitmap选入后备缓冲区的设备上下文HdcBackBuffer中，以便在该设备上下文中使用该位图。
        HOldBitmap = static_cast<HBITMAP>(SelectObject(HdcBackBuffer, HBitmap));
        ReleaseDC(Hwnd, Hdc);
    }
    break;
    case WM_PAINT: {
        BeginPaint(Hwnd, &PaintStruct);
        /*
         * 调用了 BitBlt() 函数来将一个矩形区域填充为黑色
         * 第一个是目标设备上下文句柄（在这里是 HdcBackBuffer）
         * 第二个和第三个分别为目标矩形左上角的 x 和 y 坐标（在这里都为 0）
         * 第四个和第五个分别为目标矩形的宽度和高度（在这里分别为 ClientRectWidth 和 ClientRectHeight）
         * 第六个是源设备上下文句柄（在这里为 nullptr）
         * 第七个和第八个分别为源矩形左上角的 x 和 y 坐标（在这里都为 NULL）
         * 最后一个参数是光栅操作码（在这里是 BLACKNESS）。
         */
        BitBlt(HdcBackBuffer, 0, 0, ClientRectWidth, ClientRectHeight, nullptr, NULL, NULL, BLACKNESS);

        gDevice->ClearBuffer({0.1f, 0.1f, 0.1f, 1.0f});

        const clock_t start = clock();
        {
            gDevice->Update();
            gDevice->SetMvp();
            // gDevice->DrawPrimitive(gDevice->pModel->ModelList[gModelIndex]);
            gDevice->DrawLine(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MuVector(255, 0, 0));
        }
        const clock_t stop = clock();

        const float dur = static_cast<float>(stop - start) / static_cast<float>(CLOCKS_PER_SEC);
        const std::string fps = "FPS:" + std::to_string(1.0f / dur);
        SetTextColor(HdcBackBuffer, RGB(255, 255, 255));
        SetBkMode(HdcBackBuffer, TRANSPARENT);
        TextOutX(HdcBackBuffer, 5, 5 + 20 * 1, fps);
        TextOutX(HdcBackBuffer, 5, 5 + 20 * 2, "Space: solid / edge / texture");
        TextOutX(HdcBackBuffer, 5, 5 + 20 * 3, "N: next scene");
        TextOutX(HdcBackBuffer, 5, 5 + 20 * 4, "MouseWheel: Scale");
        /*
         * 调用了 BitBlt() 函数来将内存设备上下文中的位图复制到窗口的设备上下文中
         * 第一个是目标设备上下文句柄（在这里是 PaintStruct.hdc），
         * 第二个和第三个分别为目标矩形左上角的 x 和 y 坐标（在这里都为 0）
         * 第四个和第五个分别为目标矩形的宽度和高度（在这里分别为 ClientRectWidth 和 ClientRectHeight）
         * 第六个是源设备上下文句柄（在这里是 HdcBackBuffer）
         * 第七个和第八个分别为源矩形左上角的 x 和 y 坐标（在这里都为 0）
         * 最后一个参数是光栅操作码 SRCCOPY 是 Windows API 中定义的一个光栅操作码，它的值为 0x00CC0020。当使用 BitBlt() 函数进行位图传输时，如果指定了 SRCCOPY 光栅操作码，则表示将源矩形中的像素直接复制到目标矩形中。
         */
        BitBlt(PaintStruct.hdc, 0, 0, ClientRectWidth, ClientRectHeight, HdcBackBuffer, 0, 0, SRCCOPY);

        EndPaint(Hwnd, &PaintStruct);
    }
    break;
    case WM_DESTROY:
        SelectObject(HdcBackBuffer, HOldBitmap);
        DeleteDC(HdcBackBuffer);
        DeleteObject(HBitmap);
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE: {
        const int xPos = GET_X_LPARAM(LParam);
        const int yPos = GET_Y_LPARAM(LParam);

        if ((WParam & MK_LBUTTON) != 0)
        {
            const float dx = MathSet::convertToRadians(0.25f * static_cast<float>(xPos - gDevice->lastMousePos.x));
            const float dy = MathSet::convertToRadians(0.25f * static_cast<float>(yPos - gDevice->lastMousePos.y));
            gDevice->mTheta += dx;
            gDevice->mPhi += dy;
            gDevice->mPhi = MathSet::clamp(gDevice->mPhi, 0.1f, 2 * PI - 0.1f);
            InvalidateRect(Hwnd, nullptr, false);
        }
        gDevice->lastMousePos.x = xPos;
        gDevice->lastMousePos.y = yPos;
    }
    break;
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN: {
        gDevice->lastMousePos.x = GET_X_LPARAM(LParam);
        gDevice->lastMousePos.y = GET_Y_LPARAM(LParam);
        SetCapture(Hwnd);
    }
    break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        ReleaseCapture();
        break;
    case WM_MOUSEWHEEL: {
        SetFocus(Hwnd);
        const float zDelta = static_cast<short>(HIWORD(WParam)) / 120.f * 0.02f;
        gDevice->mZoom += zDelta;
        gDevice->mScale = MathSet::scaleMatrix(gDevice->mZoom, gDevice->mZoom, gDevice->mZoom);
        InvalidateRect(Hwnd, nullptr, false);
    }
    break;
    case WM_KEYDOWN: {
        switch (WParam)
        {
        case VK_SPACE: {
            if (gDevice->renderMode == RenderMode::RenderStateWireFrame)
                gDevice->renderMode = RenderMode::RenderStateColor;
            else if (gDevice->renderMode == RenderMode::RenderStateColor)
                gDevice->renderMode = RenderMode::RenderStateTexture;
            else
                gDevice->renderMode = RenderMode::RenderStateWireFrame;
            gDevice->ClearBuffer({1.0f, 0.0f, 0.0f, 1.0f});
            InvalidateRect(Hwnd, nullptr, false);
        }
        break;
        case 78: {
            gModelIndex = (gModelIndex + 1) % gDevice->pModel->ModelList.size();
            InvalidateRect(Hwnd, nullptr, false);
        }
        break;
        default: break;
        }
    }
    break;
    default:;
    }
    return DefWindowProc(Hwnd, Message, WParam, LParam);
}
