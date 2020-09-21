/********************************
	file:DeviceL.h
	渲染设备
	包含2D绘制、视口变换、简单裁剪、深度测试、光栅化、数据插值
	author: Lcy
	date:2019-3-27
*********************************/
#pragma once
#include "math.h"
#include "Model.h"
#include "Texture.h"
#include <memory>   // std::unique_ptr support
//struct Device_L;
//extern Device_L *device;
#include <Eigen/Eigen>
enum RanderMode
{
	Rander_State_WireFrame = 0,
	Rander_State_Color,
	Rander_State_Texture
};

struct Device_L
{

	int m_width,m_height;   // 窗口宽、高
	RanderMode m_randermode;  //渲染模式1线框2颜色3纹理
	UINT* m_pFrameBuffer;
	float* m_zBuffer;  //z 缓存
	Eigen::Matrix4f mWorld, mView, mProj,mvp,mScale;//空间转换矩阵
	float mTheta;//鼠标控制
	float mPhi;
	float mRadius; 
	float m_Zoom ;//scale
	UINT m_Optimized ; //光栅化优化开关
	Model* m_pModel;//模型
	TextureL* m_pTexture;
	POINT mLastMousePos;

public:
	Device_L() {}
	~Device_L() {};
	//初始化
	void initialize(int w,int h,void* framebuf);
	void Destroy();//销毁
	void ClearBuffer(const Eigen::Vector4f& color);

	//主渲染函数
	void update();//状态更新
	void SetCameraPos(const Eigen::Vector4f &look, const Eigen::Vector4f &at);//设置相机
	void SetFrustum(float fov,float ratio,float n,float f);//设置视锥体
	void SetMvp();//设置变换矩阵
	void DrawPrimitive(Mesh* pMesh);//图元绘制

	bool Clip(Eigen::Vector4f& pos);//奇次裁剪空间简单裁剪
	void NDC2Screen(Vertex& vertexout);//透视除法&视口映射
	//透视校正插值
	void Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const Eigen::Vector4f& Vw);
	//void LoadBmp(char* filename);
	
	//光栅化函数
	//void DrawLine1(int x1, int y1, int x2, int y2, const Eigen::Vector4f& color);
	void DrawPoint(int x, int y, const UINT color,const float z);
	void DrawLine(int x0, int y0, int x1, int y1, const Eigen::Vector4f& color);
	void DrawLineX(Eigen::Vector4f& p0, Eigen::Vector4f& p1,const Eigen::Vector4f& color);
	bool TriangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, Eigen::Vector4f& Vw);
	inline bool BackFaceCulling(const Eigen::Vector4f &p0, const Eigen::Vector4f &p1, const Eigen::Vector4f &p2) {
		return (p0.dot((p2 - p0).cross(p1 - p0)) > 0);}
	void FillTriangleC(Vertex& v0,Vertex&v1,Vertex&v2);//顶点色模式
	void FillTriangleT(Vertex& v0, Vertex&v1, Vertex&v2);//纹理模式
	void Rasterize(Vertex& v0,Vertex& v1,Vertex& v2);
};