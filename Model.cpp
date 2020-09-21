#pragma once
#include "Model.h"
#include "Colors.h"

void Model::CreateTriangle3D()
{
	/*
	//�����Ա������ʹ��new�����ڴ�ʱ���漰����Ķ�̬�ڴ����
	//��Ҫ���������������ͷ�new�����Ķ��ڴ档
	//���߿���ʹ��std::unique_ptr����ָ��������,����ʹ������ָ��
	//����ָ��ο� https://zh.cppreference.com/w/cpp/memory/unique_ptr
	//��������ָ�������Ƿ������⣬�������
	*/
	Mesh* meshTri =new Mesh();
	Vertex v[5];
	//��������׶,��D3D���㡢��������,5������,18������
	v[0] = Vertex({0.f, 1.f, 0.f,1.f}, Colors_L::Green, {0.5,0.f});//����v0
	v[1] = Vertex({ -1.f, -1.f, -1.f,1.f }, Colors_L::Blue, {0.f,1.f});//��ǰ��v1
	v[2] = Vertex({ 1.f, -1.f, -1.f ,1.f}, Colors_L::Magenta, { 1.f,1.f });//��ǰ��v2
	v[3] = Vertex({ 1.f, -1.f, 1.f ,1.f}, Colors_L::Cyan,{0.f,1.f});//�Һ��v3
	v[4] = Vertex({ -1.f, -1.f, 1.f ,1.f}, Colors_L::Azure, {1.f,1.f});//����v4
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

	//��������������Ⱦʱ����Ҫ6��24���������ݣ�������������UV���꣬
	//������ʹ�þ��м�����ͼ����bmpʱ���ᵼ��ͼ���������Ť��,�����в���
	//�߿�ģʽ�Ͷ���ɫģʽ�򲻺�UV����ֵ����Ⱦֻ��Ҫ�ṩ8������
	//ǰ��
	v[0] = Vertex({ -1.f, -1.f, -1.f,1.f }, Colors_L::Blue, {0.f,1.f});//����
	v[1] = Vertex({ -1.f, 1.f, -1.f ,1.f }, Colors_L::YellowGreen, {0.f,0.f});//����
	v[2] = Vertex({ 1.f, 1.f, -1.f ,1.f }, Colors_L::Cyan, {1.f,0.f});//����
	v[3] = Vertex({ 1.f, -1.f, -1.f,1.f }, Colors_L::Magenta, {1.f,1.f});//����
	//���
	v[4] = Vertex({ -1.f, -1.f, 1.f,1.f }, Colors_L::Lavender, {1.f,1.f});//����
	v[5] = Vertex({ 1.f, -1.f, 1.f,1.f }, Colors_L::DarkSlateGray, {0.f,1.f});//����
	v[6] = Vertex({ 1.f, 1.f, 1.f ,1.f }, Colors_L::DarkOrange, {0.f,0.f});//����
	v[7] = Vertex({ -1.f, 1.f, 1.f ,1.f }, Colors_L::Green, {1.f,0.f});//����
	//�ϲ�
	v[8] = Vertex({ -1.f, 1.f, -1.f,1.f }, Colors_L::Magenta, { 0.f,1.f });//ǰ��
	v[9] = Vertex({ -1.f, 1.f, 1.f,1.f }, Colors_L::Cyan, { 0.f,0.f });//����
	v[10] = Vertex({ 1.f, 1.f, 1.f ,1.f }, Colors_L::DarkOrange, { 1.f,0.f });//����
	v[11] = Vertex({ 1.f, 1.f, -1.f ,1.f }, Colors_L::Green, { 1.f,1.f });//ǰ��
	//�²�
	v[12] = Vertex({ -1.f, -1.f, -1.f,1.f }, Colors_L::Blue, { 1.f,1.f });//ǰ��
	v[13] = Vertex({ 1.f, -1.f, -1.f,1.f }, Colors_L::YellowGreen, { 0.f,1.f });//ǰ��
	v[14] = Vertex({ 1.f, -1.f, 1.f ,1.f }, Colors_L::DarkSlateGray,{ 0.f,0.f });//����
	v[15] = Vertex({ -1.f, -1.f, 1.f ,1.f }, Colors_L::Lavender, { 1.f,0.f });//����
	//���
	v[16] = Vertex({ -1.f, -1.f, 1.f,1.f }, Colors_L::Magenta, { 0.f,1.f });//����
	v[17] = Vertex({ -1.f, 1.f,1.f,1.f }, Colors_L::Blue, { 0.f,0.f });//����
	v[18] = Vertex({ -1.f, 1.f, -1.f ,1.f }, Colors_L::Lavender, { 1.f,0.f });//ǰ��
	v[19] = Vertex({ -1.f, -1.f, -1.f ,1.f }, Colors_L::Green, { 1.f,1.f });//ǰ��
	//�Ҳ�
	v[20] = Vertex({ 1.f, -1.f, -1.f,1.f }, Colors_L::Cyan, { 0.f,1.f });//ǰ��
	v[21] = Vertex({ 1.f, 1.f, -1.f,1.f }, Colors_L::YellowGreen, { 0.f,0.f });//ǰ��
	v[22] = Vertex({ 1.f, 1.f, 1.f ,1.f }, Colors_L::DarkSlateGray, { 1.f,0.f });//����
	v[23] = Vertex({ 1.f, -1.f, 1.f ,1.f }, Colors_L::DarkOrange, { 1.f,1.f });//����

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
