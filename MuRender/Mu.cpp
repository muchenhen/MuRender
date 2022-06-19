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
//����������תʱ���͵����㴰��
//https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel

//ȫ�ֱ���
const int screenWidth = 800;//������Ļ���
const int screenHeight = 600;//������Ļ�߶�

static HWND ghwnd;//���ھ��

string windowTitle = "MuRender";//���ڱ���

unsigned int gModelIndex = 1;//ģ������ 0��box 1��triangle

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

	//������ע��
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
	//��ȡ�ͻ���������δ�С
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

	//˫���� ref:��AI Techniques for Game Programming��
	static int cxClient, cyClient;
	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;

	switch (message)
	{
	case WM_CREATE:
	{
		//��ȡ�ͻ����ߴ�
		RECT rect;
		GetClientRect(hwnd, &rect);
		cxClient = rect.right - rect.left;
		cyClient = rect.bottom - rect.top;
		BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),cxClient,cyClient , 1, 32, BI_RGB,cxClient * cyClient * (DWORD)4, 0, 0, 0, 0 } };
		LPVOID ptr;
		//��ȡ�豸DC
		hdc = GetDC(hwnd);
		//��ȡ�ڴ�DC
		hdcBackBuffer = CreateCompatibleDC(hdc);

		hBitmap = CreateDIBSection(hdcBackBuffer, &bi, DIB_RGB_COLORS,
			&ptr, 0, 0);//�ڴ�λͼ����ָ��ptr,���ڳ�ʼ��framebuffer;
		if (!hBitmap)
		{
			MessageBox(nullptr, TEXT("create dib section failed!"), TEXT("error"), MB_OK);
			return 0;
		}
		//��ʼ���豸
		device->initialize(cxClient, cyClient, ptr);
		ptr = nullptr;
		//��ȡģ��
		device->pModel->creatBox3D();
		device->pModel->creatTriangle3D();
		//device->pModel->objCube();
		//device->pModel->objCube2();
		//device->pModel->objBuffer("J:/SoftRender/Model/diablo3_pose.obj");
		device->pModel->objBuffer("./Model/diablo3_pose.obj");
		//��������
		device->pTexture->LoadBmp("./Texture/Res/diablo3_pose_diffuse.bmp");
		//bitmapװ���ڴ�DC
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
		//�ͷ�DC
		ReleaseDC(hwnd, hdc);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		//�ú�ɫ���backbuffer
		BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, BLACKNESS);
		device->clearBuffer({0.1f,0.1f,0.1f,1.0f});
		//----------------------
		//������ִ�����л�ͼ����
		clock_t start = clock();//paint start clock
		{
			device->update();//��ʼ�ӿ��Լ�״̬����
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
		//backbuffer���ݴ���frontbuffer
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
			// �����������������������������½Ƕȡ�
			device->mTheta += dx;
			device->mPhi += dy;
			// ���ƽǶ� mPhi��
			device->mPhi = MathSet::clamp(device->mPhi, 0.1f, 2 * Pi - 0.1f);
			//״̬����
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