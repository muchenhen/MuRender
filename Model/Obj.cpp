#include "Obj.h"

void Obj::createCubeObj()
{
	//顶点位置
	posList.push_back(Vector4(-1.0f, -1.0f, -1.0f, 1.0f));
	posList.push_back(Vector4(1.0f, -1.0f, -1.0f, 1.0f));
	posList.push_back(Vector4(1.0f, 1.0f, -1.0f, 1.0f));
	posList.push_back(Vector4(-1.0f, 1.0f, -1.0f, 1.0f));
	posList.push_back(Vector4(-1.0f, -1.0f, 1.0f, 1.0f));
	posList.push_back(Vector4(1.0f, -1.0f, 1.0f, 1.0f));
	posList.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	posList.push_back(Vector4(-1.0f, 1.0f, 1.0f, 1.0f));
	//顶点UV
	uvList.push_back(Vector4(0.0f, 0.0f));
	uvList.push_back(Vector4(0.0f, 1.0f));
	uvList.push_back(Vector4(1.0f, 0.0f));
	uvList.push_back(Vector4(1.0f, 1.0f));
	//位置索引
	faceIndexPos = { 
		0,2,1,	0,3,2,//front
		4,7,6,	4,6,5,//back
		0,7,4,	0,7,3,//left
		1,6,5,	1,6,2,//right
		2,7,6,	2,7,3,//up
		0,5,1,	0,5,4//bottom
	};

	//uv索引
	faceIndexUV = {
		0,2,1,	0,3,2,
		0,3,2,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,3,	0,2,1,
		0,2,1,	0,2,3
	};

	//按照位置索引和uv索引组成vertex
	
	for (int i = 0; i < faceIndexPos.size(); i++)
	{
		Vertexs.push_back(Vertex(posList[faceIndexPos[i]], ColorsL::AliceBlue, uvList[faceIndexUV[i]]));
	}

}

bool Obj::LoadObj(string filename)
{
	ifstream fin;
	fin.open(filename, ios_base::in);

	if (!fin)
		return false;

	string flag;
	char line[100];
	stringstream ss;

	fin.getline(line, 100);

	while (fin.peek() != EOF)
	{
		ss.clear();
		ss.str(line);
		ss >> flag;

		if (flag == "v")
		{
			Vector3 v;
			ss >> v.x >> v.y >> v.z;
			posList.push_back(Vector4(v));
		}
		else if (flag == "vt")
		{
			Vector3 vn;
			ss >> vn.x >> vn.y >> vn.z;
			uvList.push_back(Vector4(vn));
		}
		else if (flag == "f")
		{
			char s1[20], s2[20], s3[20];
			ss >> s1 >> s2 >> s3;

			vector<string> tr1 = split(s1, "/");
			vector<string> tr2 = split(s2, "/");
			vector<string> tr3 = split(s3, "/");

			faceIndexPos.push_back(stoi(tr1[0])-1);
			faceIndexPos.push_back(stoi(tr2[0])-1);
			faceIndexPos.push_back(stoi(tr3[0])-1);

			faceIndexUV.push_back(stoi(tr1[1])-1);
			faceIndexUV.push_back(stoi(tr2[1])-1);
			faceIndexUV.push_back(stoi(tr3[1])-1);
		}
		fin.getline(line, 100);
	}
	fin.close();



	return true;
}

vector<string> Obj::split(char* data, const char* d)
{
	vector<string> strlist;
	char* buff = NULL;
	char* p = strtok_s(data, d, &buff);
	while (p)
	{
		strlist.push_back(p);
		p = strtok_s(NULL, d, &buff);
	}
	return strlist;
}