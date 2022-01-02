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
* Objģ���ļ���
* ��ȡһ��Obj�ļ�����һ��Model����
* 1����ȡ���ж�����Ϣ
* 2������������
*/
class Model
{
private:
	//����
	std::vector<Vector3f> Vertexs;
	//��
	std::vector<Face> Faces;

public:
	Model(const char* FileName);
	~Model();

	const int& GetVertexNumber();
	const int& GetFacesNumber();
};

