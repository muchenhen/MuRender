/*************************************
	file:Mesh.h

	ģ��mesh
	��װ����ṹ����������
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

	//LoadMesh��������ȡģ��mesh�ĵ�
	//Mesh* LoadMesh(std::string filename)
	//����������mesh
	void CreateTriangle3D();
	//��������mesh
	void CreateBox3D();

};



