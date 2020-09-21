/**************************************************************
���դ����Ⱦ��ʵ��cpp
	������ˮ�߼�����
	1.��ѧʵ��--->math.h��Matrix.h
	2.ͼԪװ��--->mesh.h
	3.�豸��ʼ�����������㡢���ߺ�������Ⱦ�������̺���--->Device_L.h
		�������μ���geometry stage(ģ�;ֲ��ռ�L-->����ռ�w-->����ռ�v-->��βü��ռ�p--->�ӿ�
		��դ��rasterizer stage--->1 �߿�ģʽ--Bresenham���ٻ�ֱ���㷨
								2 ����ɫ���ģʽ --edge function inside-outside test-->��������͸��У����ֵ->���
								3 ����ģʽ--����ӳ��
								4.����(�ݲ�ʵ��)
	4.���ز���pixel stage--->1.����ӳ�䡢filtering�������(�ݲ�ʵ��)��2.blending(�ݲ�ʵ��) 
	5.win����������

****************************************************************/
#pragma once
#include "DeviceL.h"
#include <Eigen/Eigen>
#include <windowsx.h> //support GET_X_LPARAM()/GET_Y_LPARAM()
//ref https://docs.microsoft.com/en-us/windows/desktop/api/windowsx/nf-windowsx-get_x_lparam
#include <time.h>
#define WM_MOUSEWHEEL 0x020A
//#pragma comment(lib,"gdi32.lib")
//ȫ�ֱ���
const int screen_w = 800;
const int screen_h = 600;
UINT gModelIndex = 1; //mesh index 0=box,1=triangle
//float RotDegree = 1.f; //�����Ƕ�
static HWND ghwnd;   //���ھ��

std::unique_ptr<Device_L> device = std::make_unique<Device_L>();
void TextOutX(HDC hdc, int x, int y, const std::string str)
{
	const char* Pstr = str.c_str();
	TextOut(hdc, x, y, Pstr, str.size());
}
LRESULT CALLBACK OnEvent(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdline, int showCmd)
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
	RECT rc = { 0,0,screen_w,screen_h };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	HWND ghwnd = CreateWindow(TEXT("MainWnd"), TEXT("MuRender"),
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
		width,height, NULL, NULL, wc.hInstance, NULL);
	if(!ghwnd)
	{
		MessageBox(0, TEXT("CreateWindow Failed."), 0, 0);
		return false;
	}
	
	ShowWindow(ghwnd, SW_SHOW);
	UpdateWindow(ghwnd);

	MSG msg = {0};
	while (GetMessage(&msg,NULL,0,0))
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
			cxClient = rect.right-rect.left;
			cyClient = rect.bottom-rect.top;
			BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),cxClient,cyClient , 1, 32, BI_RGB,cxClient * cyClient * 4, 0, 0, 0, 0 } };
			LPVOID ptr;
			//��ȡ�豸DC
			hdc = GetDC(hwnd);
			//��ȡ�ڴ�DC
			hdcBackBuffer = CreateCompatibleDC(hdc);

			hBitmap = CreateDIBSection(hdcBackBuffer, &bi, DIB_RGB_COLORS, 
				&ptr, 0, 0);//�ڴ�λͼ����ָ��ptr,���ڳ�ʼ��framebuffer;
			if(!hBitmap)
			{
				MessageBox(nullptr, TEXT("create dib section failed!"), TEXT("error"), MB_OK);
				return 0;
			}
			//��ʼ���豸
			device->initialize(cxClient, cyClient, ptr);
			ptr = nullptr;
			//��ȡģ��
			device->m_pModel->CreateBox3D();
			device->m_pModel->CreateTriangle3D();
			//��������
			device->m_pTexture->LoadBmp("J:/MuRender/res/NagisaKaworu.bmp");
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
			BitBlt(hdcBackBuffer,0,0,cxClient,cyClient,NULL,NULL,NULL,BLACKNESS);
			device->ClearBuffer({0.f,0.f,0.f,1.f});
			//----------------------
			//������ִ�����л�ͼ����
			clock_t start = clock();//paint start clock
			{
			device->update();//��ʼ�ӿ��Լ�״̬����
			device->SetMvp();
			device->DrawPrimitive(device->m_pModel->ModelList[gModelIndex]);
			}
			clock_t stop = clock();//paint end clock
			//calculate fps
			float dur = (float)(stop - start) / (float)CLOCKS_PER_SEC;
			std::string fps ="FPS:"+ std::to_string( 1.0f / dur);
			SetTextColor(hdcBackBuffer, RGB(255, 255, 255));
			SetBkMode(hdcBackBuffer, TRANSPARENT);
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 1,fps);
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 2, "Space: �߿�/������ɫ/����ӳ��");
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 3, "N: �л�ģ��");
			TextOutX(hdcBackBuffer, 5, 5 + 20 * 4, "MouseWheel: ����");
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
				float dx = Math_L::ConvertToRadians(0.25f*static_cast<float>(xPos - device->mLastMousePos.x));
				float dy = Math_L::ConvertToRadians(0.25f*static_cast<float>(yPos - device->mLastMousePos.y));
				// Update angles based on input to orbit camera around box.
				device->mTheta += dx;
				device->mPhi += dy;
				// Restrict the angle mPhi.
				device->mPhi = Math_L::Clamp(device->mPhi, 0.1f, 2*Math_L::Pi - 0.1f);
				//״̬����
				//device->update();
				InvalidateRect(hwnd, NULL, false);
			}
			device->mLastMousePos.x = xPos;
			device->mLastMousePos.y = yPos;
		}
			break;
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		{
			device->mLastMousePos.x = GET_X_LPARAM(lParam);
			device->mLastMousePos.y = GET_Y_LPARAM(lParam);
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
			float zDelta = (short)HIWORD(wParam)/120.f*0.1f;
			device->m_Zoom += zDelta;
			device->mScale = Math_L::MatrixSetScale(device->m_Zoom, device->m_Zoom, device->m_Zoom);
			InvalidateRect(hwnd, NULL, false);
		}
			break;
		case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_SPACE:
			{
				if (device->m_randermode == Rander_State_WireFrame)
					device->m_randermode = Rander_State_Color;
				else if (device->m_randermode == Rander_State_Color)
					device->m_randermode = Rander_State_Texture;
				else
					device->m_randermode = Rander_State_WireFrame;
				device->ClearBuffer({ 0.f,0.f,0.f,1.f });
				InvalidateRect(hwnd, NULL, false);
			}
			break;
			case 78://"N"
			{
				gModelIndex = (gModelIndex + 1) % device->m_pModel->ModelList.size();
				InvalidateRect(hwnd, NULL, false);
			}
			break;

			}//switch
		
		}//wm_keydown
		break;
		
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
	
}





