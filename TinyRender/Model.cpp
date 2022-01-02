#include "Model.h"

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
			std::vector<Vector3i> f;
			Vector3i tmp;
			iss >> skip;
			while (iss >> tmp[0] >> skip >> tmp[1] >> skip >> tmp[2]) {
				for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
				f.push_back(tmp);
			}
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
