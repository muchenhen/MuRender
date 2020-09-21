#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <Eigen/Eigen>
#include "VectorL.h"
#include "math.h"

struct TextureL
{
	int width;
	int height;
	UINT* data;

	//����BMP
	bool LoadBmp(const std::string& filename);
	//����ӳ��
	UINT SampColor(Eigen::Vector4f& tc);
};

