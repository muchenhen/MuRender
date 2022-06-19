#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "Vector.h"
#include "MathUnion.h"

class Texture
{
public:
	//宽度信息
	int width;
	//高度信息
	int height;
	//数据
	unsigned int* data;

	Texture();
	~Texture();

	//加载BMP
	bool LoadBmp(const std::string& filename);
	//纹理映射
	unsigned SampColor(Vector4& tc);
};