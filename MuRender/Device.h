#pragma once
#include <memory>
#include "MathUnion.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Vertex.h"

enum class RenderMode {
	RenderStateWireFrame = 0,//�߿�ģʽ
	RenderStateColor = 1,//������ɫģʽ
	RenderStateTexture = 2//��ͼģʽ
};

class Device
{
public:
	//���ڸ߶�
	int width;
	//���ڿ��
	int height;
	//��Ⱦģʽ
	RenderMode renderMode;
	//֡����
	unsigned int* pointFrameBuffer;
	//z����
	float* zBuffer;
	//ת������������ϵ
	Matrix mWorld;
	//ת�����ӽ�����ϵ
	Matrix mView;
	//͸��ͶӰ����
	Matrix mProj;
	//�ռ�ת������˻�
	Matrix mMvp;
	//���ž���
	Matrix mScale;
	/*������*/
	float mTheta;
	float mPhi;
	float mRadius;
	float mZoom;
	//��դ���Ż�
	unsigned int optimized;
	//ģ��
	Model* pModel;
	//��ͼ
	Texture* pTexture;
	//��һ�����λ��
	POINT lastMousePos;

	Device() = default;
	~Device();

	//��ʼ��
	void Initialize(int W, int H, void* FrameBuffer);
	
	//����
	void destroy();

	//��ջ�����
	void clearBuffer(const Vector4& color);

	/*
	����Ⱦ����
	*/
	//״̬����
	void update();

	//�������λ��
	void setCameraPos(const Vector4& look, const Vector4& at);

	//������׶��
	void setFrustum(float fov,float ratio,float n, float f);

	//���ñ任����
	void setMVP();

	//ͼԪ����
	void drawPrimitive(Mesh* mesh);

	//��βü��ռ�򵥲ü�
	bool clip(Vector4& pos);
	//͸�ӳ������ӿ�ӳ��
	void ndc2Screen(Vertex& vertexout);

	//͸�ӽ�����ֵ
	void Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const Vector4& vw);

	//��դ������
	//���㺯��
	void drawPoint(int x, int y, const unsigned int color, const float z);

	//���ߺ���
	void drawLine(int x0, int y0, int x1, int y1, const Vector4& color);
	void drawLine(const Vector4& p0, const Vector4& p1, const Vector4& color);

	//�����μ��
	bool triangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, Vector4& Vw);

	//��������
	bool backFaceCulling(const Vector4& p0, const Vector4& p1, const Vector4& p2);

	//����ɫ���ģʽ
	void fillTriangleVertexColor(Vertex& v0, Vertex& v1, Vertex& v2);

	//�������ģʽ
	void fillTriangleTexture(Vertex& v0, Vertex& v1, Vertex& v2);

	//��դ��
	void Rasterize(Vertex& v0, Vertex& v1, Vertex& v2);
};

