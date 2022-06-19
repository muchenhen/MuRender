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

string windowTitle = "MuRender"; //窗口标题

unsigned int gModelIndex = 1; //模型索引 0是box 1是triangle

unique_ptr<Device> gDevice = make_unique<Device>();

LPCWSTR StringToLPCWSTR(const std::string Orig)
{
	const size_t origsize = Orig.length() + 1;
	
	size_t convertedChars = 0;
	const auto wcstring = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (Orig.length() - 1)));
	mbstowcs_s(&convertedChars, wcstring, origsize, Orig.c_str(), _TRUNCATE);

	return wcstring;
}

void TextOutX(HDC hdc, int x, int y, const std::string str)
{
	const LPCWSTR Pstr = StringToLPCWSTR(str);
	TextOut(hdc, x, y, Pstr, str.size());
}

LRESULT CALLBACK OnEvent(HWND, unsigned int, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdline, int showCmd)
{
	//窗口类注册
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = static_cast<WNDPROC>(OnEvent);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = TEXT("MainWnd");

	if (!RegisterClass(&wc))
	{
		MessageBox(nullptr, TEXT("RegisterClass Failed."), nullptr, 0);
		return false;
	}
	//获取客户区所需矩形大小
	RECT rc = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	const int width = rc.right - rc.left;
	const int height = rc.bottom - rc.top;

	const HWND ghwnd = CreateWindow(TEXT("MainWnd"), TEXT("MuRender"),
	                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
	                                width, height, NULL, NULL, wc.hInstance, NULL);
	if (!ghwnd)
	{
		MessageBox(nullptr, TEXT("CreateWindow Failed."), nullptr, 0);
		return false;
	}

	ShowWindow(ghwnd, SW_SHOW);
	UpdateWindow(ghwnd);

	MSG msg = {nullptr};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK OnEvent(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	//双缓存 ref:《AI Techniques for Game Programming》
	static int cxClient, cyClient;
	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;

	switch (message)
	{
	case WM_CREATE:
		{
			//获取客户区尺寸
			RECT rect;
			GetClientRect(hwnd, &rect);
			cxClient = rect.right - rect.left;
			cyClient = rect.bottom - rect.top;
			const BITMAPINFO bi = {
				{
					sizeof(BITMAPINFOHEADER), cxClient, cyClient, 1, 32, BI_RGB,
					cxClient * cyClient * static_cast<DWORD>(4), 0, 0, 0, 0
				}
			};
			LPVOID ptr;
			//获取设备DC
			hdc = GetDC(hwnd);
			//获取内存DC
			hdcBackBuffer = CreateCompatibleDC(hdc);

			hBitmap = CreateDIBSection(hdcBackBuffer, &bi, DIB_RGB_COLORS,
			                           &ptr, nullptr, 0); //内存位图数据指针ptr,用于初始化framebuffer;
			if (!hBitmap)
			{
				MessageBox(nullptr, TEXT("create dib section failed!"), TEXT("error"), MB_OK);
				return 0;
			}
			//初始化设备
			gDevice->initialize(cxClient, cyClient, ptr);
			ptr = nullptr;
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
			hOldBitmap = static_cast<HBITMAP>(SelectObject(hdcBackBuffer, hBitmap));
			//释放DC
			ReleaseDC(hwnd, hdc);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			//用黑色填充backbuffer
			BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, nullptr, NULL, NULL, BLACKNESS);
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
			SetTextColor(hdcBackBuffer, RGB(255, 255, 255));
			SetBkMode(hdcBackBuffer, TRANSPARENT);
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 1, fps);
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 2, "Space: solid / edge / texture");
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 3, "N: next scene");
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 4, "MouseWheel: Scale");
			//backbuffer内容传到frontbuffer
			BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_DESTROY:
		SelectObject(hdcBackBuffer, hOldBitmap);
		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);
	//gDevice->Destroy();
	//delete gDevice;
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		{
			const int xPos = GET_X_LPARAM(lParam);
			const int yPos = GET_Y_LPARAM(lParam);

			if ((wParam & MK_LBUTTON) != 0)
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
				InvalidateRect(hwnd, nullptr, false);
			}
			gDevice->lastMousePos.x = xPos;
			gDevice->lastMousePos.y = yPos;
		}
		break;
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		{
			gDevice->lastMousePos.x = GET_X_LPARAM(lParam);
			gDevice->lastMousePos.y = GET_Y_LPARAM(lParam);
			SetCapture(hwnd);
		}
		break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		break;
	case WM_MOUSEWHEEL:
		{
			SetFocus(hwnd);
			const float zDelta = static_cast<short>(HIWORD(wParam)) / 120.f * 0.02f;
			gDevice->mZoom += zDelta;
			gDevice->mScale = MathSet::scaleMatrix(gDevice->mZoom, gDevice->mZoom, gDevice->mZoom);
			InvalidateRect(hwnd, nullptr, false);
		}
		break;
	case WM_KEYDOWN:
		{
			switch (wParam)
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
					InvalidateRect(hwnd, nullptr, false);
				}
				break;
			case 78: //"N"
				{
					gModelIndex = (gModelIndex + 1) % gDevice->pModel->ModelList.size();
					InvalidateRect(hwnd, nullptr, false);
				}
				break;
			default:
				break;
			} //switch
		} //wm_keydown
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
