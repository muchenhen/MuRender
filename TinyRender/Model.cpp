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
            Vec3f Vertex;
            for (int i = 0; i < 3; i++)
            {
                iss >> Vertex[i];
            }
            Vertexs.push_back(Vertex);
        }
        else if (!line.compare(0, 2, "f "))
        {
            Face face = {0, 0, 0};
            iss >> skip;
            int skipInt;
            int i = 0;
            while (iss >> face[i] >> skip >> skipInt >> skip >> skipInt)
            {
                face[i]--;
                i++;
                if (i >= 3)
                    break;
                // else
                //	iss >> skip;
            }
            Faces.push_back(face);
            // face.Print();
        }
        else if (!line.compare(0, 2, "vt"))
        {
            Vec3f VT;
            iss >> skip;
            iss >> skip;
            for (int i = 0; i < 3; i++)
            {
                iss >> VT[i];
            }
            VTs.push_back(VT);
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

std::vector<Vec3f>& Model::GetVertexs()
{
    return Vertexs;
}

std::vector<Face>& Model::GetFaces()
{
    return Faces;
}

Vec3f& Model::GetVertex(const int& Index)
{
    return Vertexs[Index];
}

Face& Model::GetFace(const int& Index)
{
    return Faces[Index];
}

Vec3f& Model::GetVT(const int& Index)
{
    return VTs[Index];
}
