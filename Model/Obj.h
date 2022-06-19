/*
作为几何体的基类
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

	//存储obj中的v 即点的位置
	vector<Vector4> posList;
	//存储obj中的vt 即纹理坐标
	vector<Vector4> uvList;
	//存储obj中的vn 即点的法向量信息
	vector<Vector4> vnList;

	//存储面的信息
	//每个顶点对于位置的索引
	vector<unsigned int> faceIndexPos;
	//每个顶点对于uv的索引
	vector<unsigned int> faceIndexUV;
	//每个点对于法向量的索引
	vector<unsigned int> faceIndexVn;

	vector<Vertex> Vertexs;

	Mesh* mesh;

	Obj() = default;
	~Obj() {	};

	//生成一个立方体
	void createCubeObj();

	bool LoadObj(string filename);

	vector<string> split(char* data, const char* d);

};
