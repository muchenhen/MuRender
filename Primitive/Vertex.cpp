#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::~Vertex()
{
}

//位置颜色构造
Vertex::Vertex(const Vector4& p, const Vector4& c)
{
	pos = p;
	color = c;
}

//位置颜色uv构造
Vertex::Vertex(const Vector4& p, const Vector4& c, const Vector4& tc)
{
	pos = p;
	color = c;
	uv = tc;
}

//四浮点位置与颜色构造
Vertex::Vertex(const float& px, const float& py, const float& pz, const Vector4& c, const float pw)
{
	pos = Vector4(px, py, pz, pw);
	color = c;
}

//更改点的位置
void Vertex::vertexPosChange(float a)
{
	pos *= a;
	pos.w = 1.0f;
}
