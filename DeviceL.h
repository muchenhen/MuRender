/********************************
	file:DeviceL.h
	��Ⱦ�豸
	����2D���ơ��ӿڱ任���򵥲ü�����Ȳ��ԡ���դ�������ݲ�ֵ
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

	int m_width,m_height;   // ���ڿ���
	RanderMode m_randermode;  //��Ⱦģʽ1�߿�2��ɫ3����
	UINT* m_pFrameBuffer;
	float* m_zBuffer;  //z ����
	Eigen::Matrix4f mWorld, mView, mProj,mvp,mScale;//�ռ�ת������
	float mTheta;//������
	float mPhi;
	float mRadius; 
	float m_Zoom ;//scale
	UINT m_Optimized ; //��դ���Ż�����
	Model* m_pModel;//ģ��
	TextureL* m_pTexture;
	POINT mLastMousePos;

public:
	Device_L() {}
	~Device_L() {};
	//��ʼ��
	void initialize(int w,int h,void* framebuf);
	void Destroy();//����
	void ClearBuffer(const Eigen::Vector4f& color);

	//����Ⱦ����
	void update();//״̬����
	void SetCameraPos(const Eigen::Vector4f &look, const Eigen::Vector4f &at);//�������
	void SetFrustum(float fov,float ratio,float n,float f);//������׶��
	void SetMvp();//���ñ任����
	void DrawPrimitive(Mesh* pMesh);//ͼԪ����

	bool Clip(Eigen::Vector4f& pos);//��βü��ռ�򵥲ü�
	void NDC2Screen(Vertex& vertexout);//͸�ӳ���&�ӿ�ӳ��
	//͸��У����ֵ
	void Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const Eigen::Vector4f& Vw);
	//void LoadBmp(char* filename);
	
	//��դ������
	//void DrawLine1(int x1, int y1, int x2, int y2, const Eigen::Vector4f& color);
	void DrawPoint(int x, int y, const UINT color,const float z);
	void DrawLine(int x0, int y0, int x1, int y1, const Eigen::Vector4f& color);
	void DrawLineX(Eigen::Vector4f& p0, Eigen::Vector4f& p1,const Eigen::Vector4f& color);
	bool TriangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, Eigen::Vector4f& Vw);
	inline bool BackFaceCulling(const Eigen::Vector4f &p0, const Eigen::Vector4f &p1, const Eigen::Vector4f &p2) {
		return (p0.dot((p2 - p0).cross(p1 - p0)) > 0);}
	void FillTriangleC(Vertex& v0,Vertex&v1,Vertex&v2);//����ɫģʽ
	void FillTriangleT(Vertex& v0, Vertex&v1, Vertex&v2);//����ģʽ
	void Rasterize(Vertex& v0,Vertex& v1,Vertex& v2);
};