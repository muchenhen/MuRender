#pragma once
#include <memory>
#include "MathUnion.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Vertex.h"

enum class RenderMode {
	RenderStateWireFrame = 0,//线框模式
	RenderStateColor = 1,//顶点着色模式
	RenderStateTexture = 2//贴图模式
};

class Device
{
public:
	//窗口高度
	int width;
	//窗口宽度
	int height;
	//渲染模式
	RenderMode renderMode;
	//帧缓冲
	unsigned int* pointFrameBuffer;
	//z缓冲
	float* zBuffer;
	//转换到世界坐标系
	Matrix mWorld;
	//转换到视角坐标系
	Matrix mView;
	//透视投影矩阵
	Matrix mProj;
	//空间转换矩阵乘积
	Matrix mMvp;
	//缩放矩阵
	Matrix mScale;
	/*鼠标控制*/
	float mTheta;
	float mPhi;
	float mRadius;
	float mZoom;
	//光栅化优化
	unsigned int optimized;
	//模型
	Model* pModel;
	//贴图
	Texture* pTexture;
	//上一个鼠标位置
	POINT lastMousePos;

	Device() = default;
	~Device();

	//初始化
	void Initialize(int W, int H, void* FrameBuffer);
	
	//销毁
	void destroy();

	//清空缓冲区
	void clearBuffer(const Vector4& color);

	/*
	主渲染函数
	*/
	//状态更新
	void update();

	//设置相机位置
	void setCameraPos(const Vector4& look, const Vector4& at);

	//设置视锥体
	void setFrustum(float fov,float ratio,float n, float f);

	//设置变换矩阵
	void setMVP();

	//图元绘制
	void drawPrimitive(Mesh* mesh);

	//齐次裁剪空间简单裁剪
	bool clip(Vector4& pos);
	//透视除法和视口映射
	void ndc2Screen(Vertex& vertexout);

	//透视矫正插值
	void Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const Vector4& vw);

	//光栅化函数
	//画点函数
	void drawPoint(int x, int y, const unsigned int color, const float z);

	//画线函数
	void drawLine(int x0, int y0, int x1, int y1, const Vector4& color);
	void drawLine(const Vector4& p0, const Vector4& p1, const Vector4& color);

	//三角形检测
	bool triangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, Vector4& Vw);

	//背面消隐
	bool backFaceCulling(const Vector4& p0, const Vector4& p1, const Vector4& p2);

	//顶点色填充模式
	void fillTriangleVertexColor(Vertex& v0, Vertex& v1, Vertex& v2);

	//纹理填充模式
	void fillTriangleTexture(Vertex& v0, Vertex& v1, Vertex& v2);

	//光栅化
	void Rasterize(Vertex& v0, Vertex& v1, Vertex& v2);
};

