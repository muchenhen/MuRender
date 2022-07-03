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
            Vec3i Vs;
            int VTs[3];
            int VNs[3];

            while (iss >> face[i] >> skip >> VTs[i] >> skip >> VNs[i])
            {
                face[i]--;
                VTs[i]--;
                VNs[i]--;
                i++;
                if (i >= 3)
                    break;
            }
            face.SetVTs(VTs);
            Faces.push_back(face);
        }
        else if (!line.compare(0, 2, "vt"))
        {
            // 读取UV信息
            Vec3f UV;
            iss >> skip;
            iss >> skip;
            for (int i = 0; i < 3; i++)
            {
                iss >> UV[i];
            }
            UVs.push_back(UV);
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

// 通过定点索引拿到顶点
Vec3f& Model::GetVertex(const int& Index)
{
    return Vertexs[Index];
}

Face& Model::GetFace(const int& Index)
{
    return Faces[Index];
}

// 通过UV索引拿到UV
Vec3f& Model::GetUV(const int& Index)
{
    return UVs[Index];
}
