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

    static int CxClient, CyClient;
    static HDC HdcBackBuffer;
    static HBITMAP HBitmap;
    static HBITMAP HOldBitmap;

    switch (Message)
    {
    case WM_CREATE: {
        RECT Rect;
        GetClientRect(Hwnd, &Rect);
        CxClient = Rect.right - Rect.left;
        CyClient = Rect.bottom - Rect.top;
        const BITMAPINFO Bi = {
            {sizeof(BITMAPINFOHEADER), CxClient, CyClient, 1, 32, BI_RGB,
             CxClient * CyClient * static_cast<DWORD>(4), 0, 0, 0, 0}};
        LPVOID Ptr;
        Hdc = GetDC(Hwnd);
        HdcBackBuffer = CreateCompatibleDC(Hdc);

        HBitmap = CreateDIBSection(HdcBackBuffer, &Bi, DIB_RGB_COLORS,
                                   &Ptr, nullptr, 0);
        if (!HBitmap)
        {
            MessageBox(nullptr, TEXT("create dib section failed!"), TEXT("error"), MB_OK);
            return 0;
        }
        gDevice->Initialize(CxClient, CyClient, Ptr);
        Ptr = nullptr;
        gDevice->pModel->CreateBox3D();
        gDevice->pModel->CreateTriangle3D();
        gDevice->pModel->ObjBuffer("./Model/diablo3_pose.obj");
        gDevice->pTexture->LoadBMP("./Texture/Res/diablo3_pose_diffuse.bmp");
        HOldBitmap = static_cast<HBITMAP>(SelectObject(HdcBackBuffer, HBitmap));
        ReleaseDC(Hwnd, Hdc);
    }
    break;
    case WM_PAINT: {
        Hdc = BeginPaint(Hwnd, &PaintStruct);
        BitBlt(HdcBackBuffer, 0, 0, CxClient, CyClient, nullptr, NULL, NULL, BLACKNESS);
        gDevice->ClearBuffer({0.1f, 0.1f, 0.1f, 1.0f});
        const clock_t start = clock();
        {
            gDevice->Update();
            gDevice->SetMvp();
            gDevice->DrawPrimitive(gDevice->pModel->ModelList[gModelIndex]);
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
        BitBlt(PaintStruct.hdc, 0, 0, CxClient, CyClient, HdcBackBuffer, 0, 0, SRCCOPY);
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
