#pragma once
#include "Maths.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

struct Face
{
	int vertexIndex1;
	int vertexIndex2;
	int vertexIndex3;

	int& operator[] (const int i)
	{
		switch (i)
		{
		case 0:
			return vertexIndex1;
			break;
		case 1:
			return vertexIndex2;
			break;
		case 2:
			return vertexIndex3;
			break;
		}
	}

	void Print()
	{
		std::cout << "(" << vertexIndex1 << ", " << vertexIndex2 << ", " << vertexIndex3 << ")\n";
	}
};


/*
* Obj模型文件类
* 读取一个Obj文件生成一个Model对象
* 1、读取所有顶点信息
* 2、解析所有面
*/
class Model
{
private:
	std::vector<Vector3f> Vertexs;
	std::vector<Face> Faces;

public:
	Model(const char* FileName);
	~Model();

	const int& GetVertexNumber();
	const int& GetFacesNumber();
	std::vector<Vector3f>& GetVertexs();
	std::vector<Face>& GetFaces();
	Vector3f& GetVertex(const int& index);
};

