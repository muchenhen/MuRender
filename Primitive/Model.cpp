#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

//����������mesh
void Model::creatTriangle3D()
{
	/*
	//�����Ա������ʹ��new�����ڴ�ʱ���漰����Ķ�̬�ڴ����
	//��Ҫ���������������ͷ�new�����Ķ��ڴ档
	//���߿���ʹ��std::unique_ptr����ָ��������,����ʹ������ָ��
	//����ָ��ο� https://zh.cppreference.com/w/cpp/memory/unique_ptr
	*/
	Mesh* meshTri = new Mesh();
	Vertex v[5];
	//��������׶
	//D3D�����붥�����
	//��18������
	v[0] = Vertex(Vector4(0.0f, 1.0f, 0.0f, 1.0f), ColorsL::Green, Vector4(0.5f, 0.0f, 0.0f, 0.0f));//�����v0
	v[1] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//��ǰ��v1
	v[2] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Magenta, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//��ǰ��v2
	v[3] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Red, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//�Һ��v3
	v[4] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Azure, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//����v4
	unsigned int ar[18] =
	{
		//front��
		0,1,2,
		//back��
		0,3,4,
		//left��
		0,4,1,
		//right��
		0,2,3,
		//bottom�� ���� ����������������
		1,3,2,
		1,4,3
	};
	meshTri->vertexBuffer.assign(v, v + 5);
	meshTri->indexBuffer.assign(ar, ar + 18);
	ModelList.push_back(meshTri);
}

//����box mesh
/*
������������Ⱦʱ����Ҫ6����24��������������������nv����
������ʹ�þ��м���ͼ�ε�bmpʱ�ᵼ���������Ť��
�߿�ģʽ�Ͷ�����ɫģʽֻ��Ҫ8������
*/
void Model::creatBox3D()
{
	Mesh* meshBox = new Mesh();
	Vertex v[24];
	/*
	front��back�棬left��right�棬top��bottom��ֱ��������������İ˸�������ɵ�
	����uv��ͬ
	*/
	//front��
	v[0] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//���� �˸����Ϊ01
	v[1] = Vertex(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//���� 02
	v[2] = Vertex(Vector4(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Cyan, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//���� 03
	v[3] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Magenta, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//���� 04
	//back��
	v[4] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Lavender, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//���� 05
	v[5] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//���� 06
	v[6] = Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//���� 07
	v[7] = Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Green, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//���� 08
	//top��
	v[8] = Vertex(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Magenta, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//ǰ�� 02
	v[9] = Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Cyan, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//���� 08
	v[10] = Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//���� 07
	v[11] = Vertex(Vector4(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Green, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//ǰ�� 03
	//bottom�� 
	v[12] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//ǰ�� 01
	v[13] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//ǰ�� 04
	v[14] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//���� 06
	v[15] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Lavender, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//���� 05
	//left��
	v[16] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Magenta, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//���� 05
	v[17] = Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Blue, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//���� 08
	v[18] = Vertex(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Lavender, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//ǰ�� 02
	v[19] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Green, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//ǰ�� 01
	//right��
	v[20] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Cyan, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//ǰ�� 04
	v[21] = Vertex(Vector4(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//ǰ�� 03
	v[22] = Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//���� 07
	v[23] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//���� 06

	///*
	for (int i = 0; i < 24; i++)
	{
		v[i].vertexPosChange(1.0f);
	}
	//*/

	unsigned int ar[36] =
	{
		//front
		0,2,1,
		0,3,2,
		//back
		4,6,5,
		4,7,6,
		//top
		8,10,9,
		8,11,10,
		//bottom
		12,14,13,
		12,15,14,
		//left
		16,18,17,
		16,19,18,
		//right
		20,22,21,
		20,23,22
	};
	meshBox->vertexBuffer.assign(v, v + 24);
	meshBox->indexBuffer.assign(ar, ar + 36);
	ModelList.push_back(meshBox);
}

void Model::objCube()
{
	Obj obj;
	//����λ��
	obj.posList.push_back(Vector4(-1.0f, -1.0f, -1.0f, 1.0f));//0
	obj.posList.push_back(Vector4( 1.0f, -1.0f, -1.0f, 1.0f));//1
	obj.posList.push_back(Vector4( 1.0f,  1.0f, -1.0f, 1.0f));//2
	obj.posList.push_back(Vector4(-1.0f,  1.0f, -1.0f, 1.0f));//3
	obj.posList.push_back(Vector4(-1.0f, -1.0f,  1.0f, 1.0f));//4
	obj.posList.push_back(Vector4( 1.0f, -1.0f,  1.0f, 1.0f));//5
	obj.posList.push_back(Vector4( 1.0f,  1.0f,  1.0f, 1.0f));//6
	obj.posList.push_back(Vector4(-1.0f,  1.0f,  1.0f, 1.0f));//7
	//����UV
	obj.uvList.push_back(Vector4(0.0f, 0.0f));
	obj.uvList.push_back(Vector4(0.0f, 1.0f));
	obj.uvList.push_back(Vector4(1.0f, 0.0f));
	obj.uvList.push_back(Vector4(1.0f, 1.0f));
	//λ������
	obj.faceIndexPos = {
		0,2,1,	0,3,2,//front
		4,7,6,	4,6,5,//back
		0,7,4,	0,7,3,//left
		1,6,5,	1,6,2,//right
		2,7,6,	2,7,3,//up
		0,5,1,	0,5,4//bottom
	};

	//uv����
	obj.faceIndexUV = {
		0,2,1,	0,3,2,
		0,3,2,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,1,	0,2,3
	};

	//����λ��������uv�������vertex

	for (int i = 0; i < obj.faceIndexPos.size(); i++)
	{
		obj.Vertexs.push_back(Vertex(obj.posList[obj.faceIndexPos[i]], ColorsL::AliceBlue, obj.uvList[obj.faceIndexUV[i]]));
	}

	Mesh* mesh = new Mesh();
	mesh->vertexBuffer = obj.Vertexs;
	for (int i = 0; i < mesh->vertexBuffer.size(); i++)
	{
		mesh->indexBuffer.push_back(i);
	}
	ModelList.push_back(mesh);
}

void Model::objCube2()
{
	Obj obj;
	//����λ��
	obj.posList.push_back(Vector4(-1.0f, -1.0f, -1.0f, 1.0f));//0
	obj.posList.push_back(Vector4(-1.0f,  1.0f, -1.0f, 1.0f));//1
	obj.posList.push_back(Vector4( 1.0f,  1.0f, -1.0f, 1.0f));//2
	obj.posList.push_back(Vector4( 1.0f, -1.0f, -1.0f, 1.0f));//3
	obj.posList.push_back(Vector4(-1.0f, -1.0f,  1.0f, 1.0f));//4
	obj.posList.push_back(Vector4( 1.0f, -1.0f,  1.0f, 1.0f));//5
	obj.posList.push_back(Vector4( 1.0f,  1.0f,  1.0f, 1.0f));//6
	obj.posList.push_back(Vector4(-1.0f,  1.0f,  1.0f, 1.0f));//7
	//����UV
	obj.uvList.push_back(Vector4(0.0f, 0.0f));
	obj.uvList.push_back(Vector4(0.0f, 1.0f));
	obj.uvList.push_back(Vector4(1.0f, 0.0f));
	obj.uvList.push_back(Vector4(1.0f, 1.0f));
	//λ������
	obj.faceIndexPos = {
		0,2,1,0,3,2,
		4,6,5,4,7,6,
		4,1,7,4,0,1,
		3,6,2,3,5,6,
		1,6,7,1,2,6,
		0,5,3,0,4,5
	};

	//uv����
	obj.faceIndexUV = {
		0,2,1,	0,3,2,
		0,3,2,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,1,	0,2,3
	};

	//����λ��������uv�������vertex

	for (int i = 0; i < obj.faceIndexPos.size(); i++)
	{
		obj.Vertexs.push_back(Vertex(obj.posList[obj.faceIndexPos[i]], ColorsL::AliceBlue, obj.uvList[obj.faceIndexUV[i]]));
	}

	Mesh* mesh = new Mesh();
	mesh->vertexBuffer = obj.Vertexs;
	for (int i = 0; i < mesh->vertexBuffer.size(); i++)
	{
		mesh->indexBuffer.push_back(i);
	}
	ModelList.push_back(mesh);
}

void Model::objBuffer(string filename)
{
	Obj obj;
	obj.LoadObj(filename);
	Mesh* mesh = new Mesh();

	//����λ��������uv�������vertex

	for (int i = 0; i < obj.faceIndexPos.size(); i++)
	{
		obj.Vertexs.push_back(Vertex(obj.posList[obj.faceIndexPos[i]], randColor(), obj.uvList[obj.faceIndexUV[i]]));
	}

	mesh->vertexBuffer = obj.Vertexs;
	for (int i = 0; i < mesh->vertexBuffer.size(); i++)
	{
		mesh->indexBuffer.push_back(i);
	}

	ModelList.push_back(mesh);
}


