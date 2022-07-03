#pragma once
#include "Maths.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "TGAImage.h"

/*
 * 作为一个Face
 * 按照顺序有 顶点索引/uv索引/法线索引
 */

struct Face
{
	// 顶点索引
    int VertexIndex[3];
	// UV索引
    int VTIndex[3];
    int VNIndex[3];

	int vertexIndex1;
	int vertexIndex2;
	int vertexIndex3;

	Face(int InvertexIndex1,
         int InvertexIndex2,
         int InvertexIndex3)
	{
        vertexIndex1 = InvertexIndex1;
        vertexIndex2 = InvertexIndex2;
        vertexIndex3 = InvertexIndex3;
	}

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

	void SetVTs(int InVTs[3])
	{
        VTIndex[0] = InVTs[0];
        VTIndex[1] = InVTs[1];
        VTIndex[2] = InVTs[2];
	}

	void SetVNs(int InVNs[3])
    {
        VNIndex[0] = InVNs[0];
        VNIndex[1] = InVNs[1];
        VNIndex[2] = InVNs[2];
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
	// 所有的顶点信息
    std::vector<Vec3f> Vertexs;
	// 所有的Face信息
	std::vector<Face> Faces;
	// 每一个顶点对应的UV信息
    std::vector<Vec3f> UVs;

public:
	Model(const char* FileName);
	~Model();

	const int& GetVertexNumber();
	const int& GetFacesNumber();
    std::vector<Vec3f>& GetVertexs();
	std::vector<Face>& GetFaces();
    Vec3f& GetVertex(const int& Index);
    Face& GetFace(const int& Index);
    Vec3f& GetUV(const int& Index);
};

