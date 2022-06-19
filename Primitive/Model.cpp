#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

//创建三角形mesh
void Model::creatTriangle3D()
{
	/*
	//在类成员函数中使用new申请内存时，涉及到类的动态内存管理，
	//需要在类析构函数中释放new出来的堆内存。
	//或者可以使用std::unique_ptr智能指针管理对象,这里使用智能指针
	//智能指针参考 https://zh.cppreference.com/w/cpp/memory/unique_ptr
	*/
	Mesh* meshTri = new Mesh();
	Vertex v[5];
	//等腰三角锥
	//D3D索引与顶点规则
	//共18个索引
	v[0] = Vertex(Vector4(0.0f, 1.0f, 0.0f, 1.0f), ColorsL::Green, Vector4(0.5f, 0.0f, 0.0f, 0.0f));//顶尖点v0
	v[1] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//左前点v1
	v[2] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Magenta, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//右前点v2
	v[3] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Red, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//右后点v3
	v[4] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Azure, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//左后点v4
	unsigned int ar[18] =
	{
		//front面
		0,1,2,
		//back面
		0,3,4,
		//left面
		0,4,1,
		//right面
		0,2,3,
		//bottom面 矩形 所以是两个三角形
		1,3,2,
		1,4,3
	};
	meshTri->vertexBuffer.assign(v, v + 5);
	meshTri->indexBuffer.assign(ar, ar + 18);
	ModelList.push_back(meshTri);
}

//创建box mesh
/*
正方体纹理渲染时，需要6个面24个顶点数据来满足纹理nv坐标
否则在使用具有几何图形的bmp时会导致团拉伸和扭曲
线框模式和顶点颜色模式只需要8个顶点
*/
void Model::creatBox3D()
{
	Mesh* meshBox = new Mesh();
	Vertex v[24];
	/*
	front和back面，left和right面，top和bottom面分别是由组成正方体的八个顶点组成的
	但是uv不同
	*/
	//front面
	v[0] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//左下 八个点记为01
	v[1] = Vertex(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//左上 02
	v[2] = Vertex(Vector4(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Cyan, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//右上 03
	v[3] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Magenta, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//右下 04
	//back面
	v[4] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Lavender, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//左下 05
	v[5] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//右下 06
	v[6] = Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//右上 07
	v[7] = Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Green, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//左上 08
	//top面
	v[8] = Vertex(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Magenta, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//前左 02
	v[9] = Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Cyan, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//后左 08
	v[10] = Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//后右 07
	v[11] = Vertex(Vector4(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Green, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//前右 03
	//bottom面 
	v[12] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//前左 01
	v[13] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//前右 04
	v[14] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//后右 06
	v[15] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Lavender, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//后左 05
	//left面
	v[16] = Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Magenta, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//后下 05
	v[17] = Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Blue, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//后上 08
	v[18] = Vertex(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Lavender, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//前上 02
	v[19] = Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Green, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//前下 01
	//right面
	v[20] = Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Cyan, Vector4(0.0f, 1.0f, 0.0f, 0.0f));//前下 04
	v[21] = Vertex(Vector4(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, Vector4(0.0f, 0.0f, 0.0f, 0.0f));//前上 03
	v[22] = Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, Vector4(1.0f, 0.0f, 0.0f, 0.0f));//后上 07
	v[23] = Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, Vector4(1.0f, 1.0f, 0.0f, 0.0f));//后下 06

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
	//顶点位置
	obj.posList.push_back(Vector4(-1.0f, -1.0f, -1.0f, 1.0f));//0
	obj.posList.push_back(Vector4( 1.0f, -1.0f, -1.0f, 1.0f));//1
	obj.posList.push_back(Vector4( 1.0f,  1.0f, -1.0f, 1.0f));//2
	obj.posList.push_back(Vector4(-1.0f,  1.0f, -1.0f, 1.0f));//3
	obj.posList.push_back(Vector4(-1.0f, -1.0f,  1.0f, 1.0f));//4
	obj.posList.push_back(Vector4( 1.0f, -1.0f,  1.0f, 1.0f));//5
	obj.posList.push_back(Vector4( 1.0f,  1.0f,  1.0f, 1.0f));//6
	obj.posList.push_back(Vector4(-1.0f,  1.0f,  1.0f, 1.0f));//7
	//顶点UV
	obj.uvList.push_back(Vector4(0.0f, 0.0f));
	obj.uvList.push_back(Vector4(0.0f, 1.0f));
	obj.uvList.push_back(Vector4(1.0f, 0.0f));
	obj.uvList.push_back(Vector4(1.0f, 1.0f));
	//位置索引
	obj.faceIndexPos = {
		0,2,1,	0,3,2,//front
		4,7,6,	4,6,5,//back
		0,7,4,	0,7,3,//left
		1,6,5,	1,6,2,//right
		2,7,6,	2,7,3,//up
		0,5,1,	0,5,4//bottom
	};

	//uv索引
	obj.faceIndexUV = {
		0,2,1,	0,3,2,
		0,3,2,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,1,	0,2,3
	};

	//按照位置索引和uv索引组成vertex

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
	//顶点位置
	obj.posList.push_back(Vector4(-1.0f, -1.0f, -1.0f, 1.0f));//0
	obj.posList.push_back(Vector4(-1.0f,  1.0f, -1.0f, 1.0f));//1
	obj.posList.push_back(Vector4( 1.0f,  1.0f, -1.0f, 1.0f));//2
	obj.posList.push_back(Vector4( 1.0f, -1.0f, -1.0f, 1.0f));//3
	obj.posList.push_back(Vector4(-1.0f, -1.0f,  1.0f, 1.0f));//4
	obj.posList.push_back(Vector4( 1.0f, -1.0f,  1.0f, 1.0f));//5
	obj.posList.push_back(Vector4( 1.0f,  1.0f,  1.0f, 1.0f));//6
	obj.posList.push_back(Vector4(-1.0f,  1.0f,  1.0f, 1.0f));//7
	//顶点UV
	obj.uvList.push_back(Vector4(0.0f, 0.0f));
	obj.uvList.push_back(Vector4(0.0f, 1.0f));
	obj.uvList.push_back(Vector4(1.0f, 0.0f));
	obj.uvList.push_back(Vector4(1.0f, 1.0f));
	//位置索引
	obj.faceIndexPos = {
		0,2,1,0,3,2,
		4,6,5,4,7,6,
		4,1,7,4,0,1,
		3,6,2,3,5,6,
		1,6,7,1,2,6,
		0,5,3,0,4,5
	};

	//uv索引
	obj.faceIndexUV = {
		0,2,1,	0,3,2,
		0,3,2,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,1,	0,2,3
	};

	//按照位置索引和uv索引组成vertex

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

	//按照位置索引和uv索引组成vertex

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


