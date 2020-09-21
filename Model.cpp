#pragma once
#include "Model.h"
#include "Colors.h"

void Model::CreateTriangle3D()
{
	/*
	//在类成员函数中使用new申请内存时，涉及到类的动态内存管理，
	//需要在类析构函数中释放new出来的堆内存。
	//或者可以使用std::unique_ptr智能指针管理对象,这里使用智能指针
	//智能指针参考 https://zh.cppreference.com/w/cpp/memory/unique_ptr
	//对于智能指针的理解是否有问题，烦请提出
	*/
	Mesh* meshTri =new Mesh();
	Vertex v[5];
	//等腰三角锥,按D3D顶点、索引规则,5个顶点,18个索引
	v[0] = Vertex({0.f, 1.f, 0.f,1.f}, Colors_L::Green, {0.5,0.f});//顶尖v0
	v[1] = Vertex({ -1.f, -1.f, -1.f,1.f }, Colors_L::Blue, {0.f,1.f});//左前点v1
	v[2] = Vertex({ 1.f, -1.f, -1.f ,1.f}, Colors_L::Magenta, { 1.f,1.f });//右前点v2
	v[3] = Vertex({ 1.f, -1.f, 1.f ,1.f}, Colors_L::Cyan,{0.f,1.f});//右后点v3
	v[4] = Vertex({ -1.f, -1.f, 1.f ,1.f}, Colors_L::Azure, {1.f,1.f});//左后点v4
	UINT ar[18] = { 
		//front face
		0,1,2,
		//back face
		0,3,4,
		//left face
		0,4,1,
		//right face
		0,2,3,
		//buttom face
		1,3,2,
		1,4,3 };
	meshTri->VertexBuffer.assign(v, v+5);
	meshTri->IndexBuffer.assign(ar, ar+18);
	ModelList.push_back(meshTri);
}
void Model::CreateBox3D()
{
	Mesh* meshBox=new Mesh();
	Vertex v[24];

	//正方体在纹理渲染时，需要6面24个顶点数据，才能满足纹理UV坐标，
	//否则在使用具有几何型图案的bmp时，会导致图案的拉伸和扭曲,请自行测试
	//线框模式和顶点色模式因不含UV坐标值，渲染只需要提供8个顶点
	//前侧
	v[0] = Vertex({ -1.f, -1.f, -1.f,1.f }, Colors_L::Blue, {0.f,1.f});//左下
	v[1] = Vertex({ -1.f, 1.f, -1.f ,1.f }, Colors_L::YellowGreen, {0.f,0.f});//左上
	v[2] = Vertex({ 1.f, 1.f, -1.f ,1.f }, Colors_L::Cyan, {1.f,0.f});//右上
	v[3] = Vertex({ 1.f, -1.f, -1.f,1.f }, Colors_L::Magenta, {1.f,1.f});//右下
	//后侧
	v[4] = Vertex({ -1.f, -1.f, 1.f,1.f }, Colors_L::Lavender, {1.f,1.f});//左下
	v[5] = Vertex({ 1.f, -1.f, 1.f,1.f }, Colors_L::DarkSlateGray, {0.f,1.f});//右下
	v[6] = Vertex({ 1.f, 1.f, 1.f ,1.f }, Colors_L::DarkOrange, {0.f,0.f});//右上
	v[7] = Vertex({ -1.f, 1.f, 1.f ,1.f }, Colors_L::Green, {1.f,0.f});//左上
	//上侧
	v[8] = Vertex({ -1.f, 1.f, -1.f,1.f }, Colors_L::Magenta, { 0.f,1.f });//前左
	v[9] = Vertex({ -1.f, 1.f, 1.f,1.f }, Colors_L::Cyan, { 0.f,0.f });//后左
	v[10] = Vertex({ 1.f, 1.f, 1.f ,1.f }, Colors_L::DarkOrange, { 1.f,0.f });//后右
	v[11] = Vertex({ 1.f, 1.f, -1.f ,1.f }, Colors_L::Green, { 1.f,1.f });//前左
	//下侧
	v[12] = Vertex({ -1.f, -1.f, -1.f,1.f }, Colors_L::Blue, { 1.f,1.f });//前左
	v[13] = Vertex({ 1.f, -1.f, -1.f,1.f }, Colors_L::YellowGreen, { 0.f,1.f });//前右
	v[14] = Vertex({ 1.f, -1.f, 1.f ,1.f }, Colors_L::DarkSlateGray,{ 0.f,0.f });//后右
	v[15] = Vertex({ -1.f, -1.f, 1.f ,1.f }, Colors_L::Lavender, { 1.f,0.f });//后左
	//左侧
	v[16] = Vertex({ -1.f, -1.f, 1.f,1.f }, Colors_L::Magenta, { 0.f,1.f });//后下
	v[17] = Vertex({ -1.f, 1.f,1.f,1.f }, Colors_L::Blue, { 0.f,0.f });//后上
	v[18] = Vertex({ -1.f, 1.f, -1.f ,1.f }, Colors_L::Lavender, { 1.f,0.f });//前上
	v[19] = Vertex({ -1.f, -1.f, -1.f ,1.f }, Colors_L::Green, { 1.f,1.f });//前下
	//右侧
	v[20] = Vertex({ 1.f, -1.f, -1.f,1.f }, Colors_L::Cyan, { 0.f,1.f });//前下
	v[21] = Vertex({ 1.f, 1.f, -1.f,1.f }, Colors_L::YellowGreen, { 0.f,0.f });//前上
	v[22] = Vertex({ 1.f, 1.f, 1.f ,1.f }, Colors_L::DarkSlateGray, { 1.f,0.f });//后上
	v[23] = Vertex({ 1.f, -1.f, 1.f ,1.f }, Colors_L::DarkOrange, { 1.f,1.f });//后下

	UINT ar[36] = {
		// front face
		0, 2, 1,
		0, 3, 2,
		// back face
		4, 6, 5,
		4, 7, 6,
		// top face
		8, 10, 9,
		8, 11, 10,
		// bottom face
		12,14,13,
		12,15,14,
		// left face
		16, 18, 17,
		16, 19, 18,
		// right face
		20, 22, 21,
		20, 23, 22
	};
	meshBox->VertexBuffer.assign(v, v+24);
	meshBox->IndexBuffer.assign(ar, ar+36);
	ModelList.push_back(meshBox);
}
