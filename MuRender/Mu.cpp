/*


*/

#include <iostream>
#include <windowsx.h>
#include <time.h>
#include <string>
#include "Device.h"
#include "MathUnion.h"
using namespace std;

#define WM_MOUSEWHEEL 0x020A
//当鼠标滚轮旋转时发送到焦点窗口
//https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel

//全局变量
const int screenWidth = 800;//窗口屏幕宽度
const int screenHeight = 600;//窗口屏幕高度

static HWND ghwnd;//窗口句柄

string windowTitle = "MuRender";//窗口标题

unsigned int gModelIndex = 1;//模型索引 0是box 1是triangle

unique_ptr<Device> device = make_unique<Device>();

LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}

void TextOutX(HDC hdc, int x, int y, const std::string str)
{
	LPCWSTR Pstr = stringToLPCWSTR(str);
	TextOut(hdc, x, y, Pstr, str.size());
}
LRESULT CALLBACK OnEvent(HWND, unsigned int, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdline, int showCmd)
{

	//窗口类注册
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)OnEvent;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT("MainWnd");

	if (!RegisterClass(&wc))
	{
		MessageBox(0, TEXT("RegisterClass Failed."), 0, 0);
		return false;
	}
	//获取客户区所需矩形大小
	RECT rc = { 0,0,screenWidth,screenHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	HWND ghwnd = CreateWindow(TEXT("MainWnd"), TEXT("MuRender"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, NULL, NULL, wc.hInstance, NULL);
	if (!ghwnd)
	{
		MessageBox(0, TEXT("CreateWindow Failed."), 0, 0);
		return false;
	}

	ShowWindow(ghwnd, SW_SHOW);
	UpdateWindow(ghwnd);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
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
		BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),cxClient,cyClient , 1, 32, BI_RGB,cxClient * cyClient * (DWORD)4, 0, 0, 0, 0 } };
		LPVOID ptr;
		//获取设备DC
		hdc = GetDC(hwnd);
		//获取内存DC
		hdcBackBuffer = CreateCompatibleDC(hdc);

		hBitmap = CreateDIBSection(hdcBackBuffer, &bi, DIB_RGB_COLORS,
			&ptr, 0, 0);//内存位图数据指针ptr,用于初始化framebuffer;
		if (!hBitmap)
		{
			MessageBox(nullptr, TEXT("create dib section failed!"), TEXT("error"), MB_OK);
			return 0;
		}
		//初始化设备
		device->initialize(cxClient, cyClient, ptr);
		ptr = nullptr;
		//获取模型
		device->pModel->creatBox3D();
		device->pModel->creatTriangle3D();
		//device->pModel->objCube();
		//device->pModel->objCube2();
		//device->pModel->objBuffer("J:/SoftRender/Model/diablo3_pose.obj");
		device->pModel->objBuffer("./Model/diablo3_pose.obj");
		//载入纹理
		device->pTexture->LoadBmp("./Texture/Res/diablo3_pose_diffuse.bmp");
		//bitmap装入内存DC
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
		//释放DC
		ReleaseDC(hwnd, hdc);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		//用黑色填充backbuffer
		BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, BLACKNESS);
		device->clearBuffer({0.1f,0.1f,0.1f,1.0f});
		//----------------------
		//在这里执行所有画图操作
		clock_t start = clock();//paint start clock
		{
			device->update();//初始视口以及状态更新
			device->setMVP();
			device->drawPrimitive(device->pModel->ModelList[gModelIndex]);
		}
		clock_t stop = clock();//paint end clock
		//calculate fps
		float dur = (float)(stop - start) / (float)CLOCKS_PER_SEC;
		std::string fps = "FPS:" + std::to_string(1.0f / dur);
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
		//device->Destroy();
		//delete device;
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		if ((wParam & MK_LBUTTON) != 0)
		{
			float dx = MathSet::convertToRadians(0.25f * static_cast<float>(xPos - device->lastMousePos.x));
			float dy = MathSet::convertToRadians(0.25f * static_cast<float>(yPos - device->lastMousePos.y));
			// 根据输入向绕箱轨道摄像机的输入更新角度。
			device->mTheta += dx;
			device->mPhi += dy;
			// 限制角度 mPhi。
			device->mPhi = MathSet::clamp(device->mPhi, 0.1f, 2 * Pi - 0.1f);
			//状态更新
			//device->update();
			InvalidateRect(hwnd, NULL, false);
		}
		device->lastMousePos.x = xPos;
		device->lastMousePos.y = yPos;
	}
	break;
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	{
		device->lastMousePos.x = GET_X_LPARAM(lParam);
		device->lastMousePos.y = GET_Y_LPARAM(lParam);
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
		float zDelta = (short)HIWORD(wParam) / 120.f * 0.02f;
		device->mZoom += zDelta;
		device->mScale = MathSet::scaleMatrix(device->mZoom, device->mZoom, device->mZoom);
		InvalidateRect(hwnd, NULL, false);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE:
		{
			if (device->renderMode == RenderMode::RenderStateWireFrame)
				device->renderMode = RenderMode::RenderStateColor;
			else if (device->renderMode == RenderMode::RenderStateColor)
				device->renderMode = RenderMode::RenderStateTexture;
			else
				device->renderMode = RenderMode::RenderStateWireFrame;
			device->clearBuffer({ 1.0f,0.0f,0.0f,1.0f });
			InvalidateRect(hwnd, NULL, false);
		}
		break;
		case 78://"N"
		{
			gModelIndex = (gModelIndex + 1) % device->pModel->ModelList.size();
			InvalidateRect(hwnd, NULL, false);
		}
		break;

		}//switch

	}//wm_keydown
	break;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);

}