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
	//顶点
	std::vector<Vector3f> Vertexs;
	//面
	std::vector<Face> Faces;

public:
	Model(const char* FileName);
	~Model();

	const int& GetVertexNumber();
	const int& GetFacesNumber();
};

