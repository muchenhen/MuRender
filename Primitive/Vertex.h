#pragma once
#include <vector>
#include <windows.h>
#include "Vector.h"

class Vertex
{
public:
	//λ��
	Vector4 pos;
	//��ɫ
	Vector4 color;
	//UV
	Vector4 uv;

	//Ĭ�Ϲ���
	Vertex();
	//��������
	~Vertex();

	//λ����ɫ����
	Vertex(const Vector4& p, const Vector4& c);

	//λ����ɫuv����
	Vertex(const Vector4& p, const Vector4& c, const Vector4& tc);

	//λ�� �����ɫ uv


	//�ĸ���λ������ɫ����
	Vertex(const float& px, const float& py, const float& pz, const Vector4& c, const float pw = 1.0f);

	//���ĵ�λ��
	void vertexPosChange(float a);
};

