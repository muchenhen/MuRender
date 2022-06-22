/*


*/

#include <iostream>
#include <windowsx.h>
#include <ctime>
#include <string>
#include "Device.h"
#include "MathUnion.h"
using namespace std;

#define WM_MOUSEWHEEL 0x020A
//当鼠标滚轮旋转时发送到焦点窗口
//https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel

//全局变量
constexpr int SCREEN_WIDTH = 800; //窗口屏幕宽度
constexpr int SCREEN_HEIGHT = 600; //窗口屏幕高度

string WindowTitle = "MuRender"; //窗口标题

unsigned int gModelIndex = 1; //模型索引 0是box 1是triangle

unique_ptr<Device> gDevice = make_unique<Device>();

LPCWSTR StringToLPCWSTR(const std::string& Orig)
{
	const size_t Origsize = Orig.length() + 1;
	
	size_t ConvertedChars = 0;
	const auto Wcstring = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (Orig.length() - 1)));
	mbstowcs_s(&ConvertedChars, Wcstring, Origsize, Orig.c_str(), _TRUNCATE);

	return Wcstring;
}

void TextOutX(const HDC HDC, const int X, const int Y, const std::string& Str)
{
	const LPCWSTR Pstr = StringToLPCWSTR(Str);
	TextOut(HDC, X, Y, Pstr, Str.size());
}

LRESULT CALLBACK OnEvent(HWND, unsigned int, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//窗口类注册
	WNDCLASS Wndclass;
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = static_cast<WNDPROC>(OnEvent);
	Wndclass.cbClsExtra = 0;
	Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = GetModuleHandle(nullptr);
	Wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	Wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	Wndclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	Wndclass.lpszMenuName = nullptr;
	Wndclass.lpszClassName = TEXT("MainWnd");

	if (!RegisterClass(&Wndclass))
	{
		MessageBox(nullptr, TEXT("RegisterClass Failed."), nullptr, 0);
		return false;
	}
	//获取客户区所需矩形大小
	RECT Rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);
	const int Width = Rect.right - Rect.left;
	const int Height = Rect.bottom - Rect.top;

	const HWND Ghwnd = CreateWindow(TEXT("MainWnd"), TEXT("MuRender"),
	                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
	                                Width, Height, NULL, NULL, Wndclass.hInstance, NULL);
	if (!Ghwnd)
	{
		MessageBox(nullptr, TEXT("CreateWindow Failed."), nullptr, 0);
		return false;
	}

	ShowWindow(Ghwnd, SW_SHOW);
	UpdateWindow(Ghwnd);

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
	PAINTSTRUCT Paintstruct;
	HDC Hdc;

	//双缓存 ref:《AI Techniques for Game Programming》
	static int CxClient, CyClient;
	static HDC HdcBackBuffer;
	static HBITMAP HBitmap;
	static HBITMAP HOldBitmap;

	switch (Message)
	{
	case WM_CREATE:
		{
			//获取客户区尺寸
			RECT Rect;
			GetClientRect(Hwnd, &Rect);
			CxClient = Rect.right - Rect.left;
			CyClient = Rect.bottom - Rect.top;
			const BITMAPINFO Bi = {
				{
					sizeof(BITMAPINFOHEADER), CxClient, CyClient, 1, 32, BI_RGB,
					CxClient * CyClient * static_cast<DWORD>(4), 0, 0, 0, 0
				}
			};
			LPVOID Ptr;
			//获取设备DC
			Hdc = GetDC(Hwnd);
			//获取内存DC
			HdcBackBuffer = CreateCompatibleDC(Hdc);

			HBitmap = CreateDIBSection(HdcBackBuffer, &Bi, DIB_RGB_COLORS,
			                           &Ptr, nullptr, 0); //内存位图数据指针ptr,用于初始化framebuffer;
			if (!HBitmap)
			{
				MessageBox(nullptr, TEXT("create dib section failed!"), TEXT("error"), MB_OK);
				return 0;
			}
			//初始化设备
			gDevice->Initialize(CxClient, CyClient, Ptr);
			Ptr = nullptr;
			//获取模型
			gDevice->pModel->creatBox3D();
			gDevice->pModel->creatTriangle3D();
			//gDevice->pModel->objCube();
			//gDevice->pModel->objCube2();
			//gDevice->pModel->objBuffer("J:/SoftRender/Model/diablo3_pose.obj");
			gDevice->pModel->objBuffer("./Model/diablo3_pose.obj");
			//载入纹理
			gDevice->pTexture->LoadBmp("./Texture/Res/diablo3_pose_diffuse.bmp");
			//bitmap装入内存DC
			HOldBitmap = static_cast<HBITMAP>(SelectObject(HdcBackBuffer, HBitmap));
			//释放DC
			ReleaseDC(Hwnd, Hdc);
		}
		break;
	case WM_PAINT:
		{
			Hdc = BeginPaint(Hwnd, &Paintstruct);
			//用黑色填充backbuffer
			BitBlt(HdcBackBuffer, 0, 0, CxClient, CyClient, nullptr, NULL, NULL, BLACKNESS);
			gDevice->clearBuffer({0.1f, 0.1f, 0.1f, 1.0f});
			//----------------------
			//在这里执行所有画图操作
			const clock_t start = clock(); //paint start clock
			{
				gDevice->update(); //初始视口以及状态更新
				gDevice->setMVP();
				gDevice->drawPrimitive(gDevice->pModel->ModelList[gModelIndex]);
			}
			const clock_t stop = clock(); //paint end clock
			//calculate fps
			const float dur = static_cast<float>(stop - start) / static_cast<float>(CLOCKS_PER_SEC);
			const std::string fps = "FPS:" + std::to_string(1.0f / dur);
			SetTextColor(HdcBackBuffer, RGB(255, 255, 255));
			SetBkMode(HdcBackBuffer, TRANSPARENT);
			TextOutX(HdcBackBuffer, 5, 5 + 20 * 1, fps);
			TextOutX(HdcBackBuffer, 5, 5 + 20 * 2, "Space: solid / edge / texture");
			TextOutX(HdcBackBuffer, 5, 5 + 20 * 3, "N: next scene");
			TextOutX(HdcBackBuffer, 5, 5 + 20 * 4, "MouseWheel: Scale");
			//backbuffer内容传到frontbuffer
			BitBlt(Paintstruct.hdc, 0, 0, CxClient, CyClient, HdcBackBuffer, 0, 0, SRCCOPY);
			EndPaint(Hwnd, &Paintstruct);
		}
		break;
	case WM_DESTROY:
		SelectObject(HdcBackBuffer, HOldBitmap);
		DeleteDC(HdcBackBuffer);
		DeleteObject(HBitmap);
	//gDevice->Destroy();
	//delete gDevice;
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		{
			const int xPos = GET_X_LPARAM(LParam);
			const int yPos = GET_Y_LPARAM(LParam);

			if ((WParam & MK_LBUTTON) != 0)
			{
				const float dx = MathSet::convertToRadians(0.25f * static_cast<float>(xPos - gDevice->lastMousePos.x));
				const float dy = MathSet::convertToRadians(0.25f * static_cast<float>(yPos - gDevice->lastMousePos.y));
				// 根据输入向绕箱轨道摄像机的输入更新角度。
				gDevice->mTheta += dx;
				gDevice->mPhi += dy;
				// 限制角度 mPhi。
				gDevice->mPhi = MathSet::clamp(gDevice->mPhi, 0.1f, 2 * Pi - 0.1f);
				//状态更新
				//gDevice->update();
				InvalidateRect(Hwnd, nullptr, false);
			}
			gDevice->lastMousePos.x = xPos;
			gDevice->lastMousePos.y = yPos;
		}
		break;
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		{
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
	case WM_MOUSEWHEEL:
		{
			SetFocus(Hwnd);
			const float zDelta = static_cast<short>(HIWORD(WParam)) / 120.f * 0.02f;
			gDevice->mZoom += zDelta;
			gDevice->mScale = MathSet::scaleMatrix(gDevice->mZoom, gDevice->mZoom, gDevice->mZoom);
			InvalidateRect(Hwnd, nullptr, false);
		}
		break;
	case WM_KEYDOWN:
		{
			switch (WParam)
			{
			case VK_SPACE:
				{
					if (gDevice->renderMode == RenderMode::RenderStateWireFrame)
						gDevice->renderMode = RenderMode::RenderStateColor;
					else if (gDevice->renderMode == RenderMode::RenderStateColor)
						gDevice->renderMode = RenderMode::RenderStateTexture;
					else
						gDevice->renderMode = RenderMode::RenderStateWireFrame;
					gDevice->clearBuffer({1.0f, 0.0f, 0.0f, 1.0f});
					InvalidateRect(Hwnd, nullptr, false);
				}
				break;
			case 78: //"N"
				{
					gModelIndex = (gModelIndex + 1) % gDevice->pModel->ModelList.size();
					InvalidateRect(Hwnd, nullptr, false);
				}
				break;
			default:
				break;
			} //switch
		} //wm_keydown
		break;
	default: 
		;
	}
	return DefWindowProc(Hwnd, Message, WParam, LParam);
}
