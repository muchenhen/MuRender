#include "Model.h"
#include <iostream>

Model::Model(const char* FileName)
{
	std::ifstream in;
	in.open(FileName, std::ifstream::in);
	if (in.fail())
		return;

	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char skip;
		if (!line.compare(0, 2, "v "))
		{
			iss >> skip;
			Vector3f Vertex;
			for (int i = 0; i < 3; i++)
			{
				iss >> Vertex[i];
			}
			Vertexs.push_back(Vertex);
		}
		else if (!line.compare(0, 2, "f "))
		{
			Face face = { 0,0,0 };
			iss >> skip;
			int skipInt;
			int i = 0;
			while (iss >> face[i] >> skip >> skipInt >> skip >> skipInt) {
				face[i]--;
				i++;
				if (i >= 3)
					break;
				//else
				//	iss >> skip;
			}
			Faces.push_back(face);
			// face.Print();
		}
	}
}

Model::~Model()
{
}

const int& Model::GetVertexNumber()
{
	return Vertexs.size();
}

const int& Model::GetFacesNumber()
{
	return Faces.size();
}

std::vector<Vector3f>& Model::GetVertexs()
{
	return Vertexs;
}

std::vector<Face>& Model::GetFaces()
{
	return Faces;
}

Vector3f& Model::GetVertex(const int& index)
{
	return Vertexs[index];
}
