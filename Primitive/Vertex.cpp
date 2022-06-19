#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::~Vertex()
{
}

//λ����ɫ����
Vertex::Vertex(const Vector4& p, const Vector4& c)
{
	pos = p;
	color = c;
}

//λ����ɫuv����
Vertex::Vertex(const Vector4& p, const Vector4& c, const Vector4& tc)
{
	pos = p;
	color = c;
	uv = tc;
}

//�ĸ���λ������ɫ����
Vertex::Vertex(const float& px, const float& py, const float& pz, const Vector4& c, const float pw)
{
	pos = Vector4(px, py, pz, pw);
	color = c;
}

//���ĵ��λ��
void Vertex::vertexPosChange(float a)
{
	pos *= a;
	pos.w = 1.0f;
}
