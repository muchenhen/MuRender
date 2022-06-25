#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::~Vertex()
{
}

Vertex::Vertex(const MuVector& p, const MuVector& c)
{
	pos = p;
	color = c;
}

Vertex::Vertex(const MuVector& p, const MuVector& c, const MuVector& tc)
{
	pos = p;
	color = c;
	uv = tc;
}

Vertex::Vertex(const float& px, const float& py, const float& pz, const MuVector& c, const float pw)
{
	pos = MuVector(px, py, pz, pw);
	color = c;
}

void Vertex::vertexPosChange(float a)
{
	pos *= a;
	pos.W = 1.0f;
}
