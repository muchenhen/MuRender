/*************************************
	file:Mesh.h

	模型mesh
	封装顶点结构、顶点索引
	author:Lcy
	date:2019-3-27
**************************************/
#pragma once
#include <vector>
#include <windows.h>
#include "VectorL.h"

struct Vertex
{
	Eigen::Vector4f pos, color, uv;
	Vertex(const Eigen::Vector4f& p,const Eigen::Vector4f& c):pos(p),color(c){}
	Vertex(const Eigen::Vector4f& p,const Eigen::Vector4f& c,const Eigen::Vector4f& tc):pos(p),color(c),uv(tc){}
	Vertex(float px,float py,float pz,const Eigen::Vector4f& c,float pw=1):pos(px,py,pz,pw),color(c) {}
	Vertex() {}
};
struct Mesh
{
	std::vector<UINT> IndexBuffer;
	std::vector<Vertex> VertexBuffer;
};
struct Model
{
	std::vector<Mesh*> ModelList;

	//LoadMesh函数，读取模型mesh文档
	//Mesh* LoadMesh(std::string filename)
	//创建三角形mesh
	void CreateTriangle3D();
	//创建方块mesh
	void CreateBox3D();

};



