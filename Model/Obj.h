/*
��Ϊ������Ļ���
*/

#pragma once
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include "MathUnion.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Colors.h"
using  namespace std;
class Obj
{
public:

	//�洢obj�е�v �����λ��
	vector<Vector4> posList;
	//�洢obj�е�vt ����������
	vector<Vector4> uvList;
	//�洢obj�е�vn ����ķ�������Ϣ
	vector<Vector4> vnList;

	//�洢�����Ϣ
	//ÿ���������λ�õ�����
	vector<unsigned int> faceIndexPos;
	//ÿ���������uv������
	vector<unsigned int> faceIndexUV;
	//ÿ������ڷ�����������
	vector<unsigned int> faceIndexVn;

	vector<Vertex> Vertexs;

	Mesh* mesh;

	Obj() = default;
	~Obj() {	};

	//����һ��������
	void createCubeObj();

	bool LoadObj(string filename);

	vector<string> split(char* data, const char* d);

};
