#pragma once
#include <vector>
#include <windows.h>
#include "Vector.h"

class Vertex
{
public:
	//位置
	Vector4 pos;
	//颜色
	Vector4 color;
	//UV
	Vector4 uv;

	//默认构造
	Vertex();
	//析构函数
	~Vertex();

	//位置颜色构造
	Vertex(const Vector4& p, const Vector4& c);

	//位置颜色uv构造
	Vertex(const Vector4& p, const Vector4& c, const Vector4& tc);

	//位置 随机颜色 uv


	//四浮点位置与颜色构造
	Vertex(const float& px, const float& py, const float& pz, const Vector4& c, const float pw = 1.0f);

	//更改点位置
	void vertexPosChange(float a);
};

