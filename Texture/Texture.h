#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "Vector.h"
#include "MathUnion.h"

class Texture
{
public:
	//�����Ϣ
	int width;
	//�߶���Ϣ
	int height;
	//����
	unsigned int* data;

	Texture();
	~Texture();

	//����BMP
	bool LoadBmp(const std::string& filename);
	//����ӳ��
	unsigned SampColor(Vector4& tc);
};