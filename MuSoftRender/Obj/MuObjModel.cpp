#include "MuObjModel.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool IsFileExist(const std::string& Filename)
{
    const std::ifstream infile(Filename);
    return infile.good();
}

MuObjModel::MuObjModel()
{
    Texture = new TGAImage();
}

MuObjModel::~MuObjModel()
{
    if (Texture)
    {
        delete Texture;
        Texture = nullptr;
    }
}

bool MuObjModel::Load(const std::string& Filename)
{
    if (!IsFileExist(Filename))
    {
        return false;
    }

    ifstream File(Filename.c_str());
    string Line;

    while (getline(File, Line))
    {
        istringstream iss(Line);
        string type;
        iss >> type;

        if (type == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            MuPoint3F Vertex(x, y, z);
            Vertices.push_back(Vertex);
        }
        else if (type == "vn")
        {
            float x, y, z;
            iss >> x >> y >> z;
            MuVector3F Normal(x, y, z);
            Normals.push_back(Normal);
        }
        else if (type == "vt")
        {
            float u, v;
            iss >> u >> v;
            MuPoint2F Texcoord(u, v);
            Texcoords.push_back(Texcoord);
        }
        else if (type == "f")
        {
            string s;
            vector<string> tokens;
            while (iss >> s)
            {
                tokens.push_back(s);
            }

            FMuObjFace Face;
            for (int i = 0; i < tokens.size(); i++)
            {
                string token = tokens[i];
                int vertexIndex, texcoordIndex, normalIndex;
                ParseVertexIndex(token, &vertexIndex, &texcoordIndex, &normalIndex);
                FMuFaceIndex FaceIndex{};
                FaceIndex.VertexIndex = vertexIndex;
                FaceIndex.TexcoordIndex = texcoordIndex;
                FaceIndex.NormalIndex = normalIndex;
                Face.Indices.push_back(FaceIndex);
            }
            FaceIndices.push_back(Face);
        }
    }

    File.close();
    return true;
}

int MuObjModel::GetFaceCount() const
{
    return FaceIndices.size();
}

int MuObjModel::GetVertexCount() const
{
    return Vertices.size();
}

FMuObjFace MuObjModel::GetFace(int I) const
{
    return FaceIndices[I];
}

MuPoint3F MuObjModel::GetVertexByIndex(int VertexIndex)
{
    return Vertices[VertexIndex];
}

MuPoint3F MuObjModel::GetScreenVertexByIndex(int VertexIndex)
{
    return ScreenVertices[VertexIndex];
}

MuPoint2F MuObjModel::GetTexcoordByIndex(int TexcoordIndex)
{
    return Texcoords[TexcoordIndex];
}

vector<MuPoint3F> MuObjModel::GetAllVertices()
{
    return Vertices;
}

bool MuObjModel::LoadTexture(const string& Filename)
{
    // 读取指定路径的TGA文件
    const bool bSuccess = Texture->read_tga_file(Filename.c_str());
    if (bSuccess)
    {
        Texture->flip_horizontally();
        Texture->flip_vertically();
    }
    return bSuccess;
}

void MuObjModel::SetScreenVertex(const vector<MuPoint3F>& ScreenVertices)
{
    this->ScreenVertices = ScreenVertices;
}

void MuObjModel::ParseVertexIndex(string Token, int* VertexIndex, int* TexcoordIndex, int* NormalIndex)
{
    string vertexString, texcoordString, normalString;
    *VertexIndex = *TexcoordIndex = *NormalIndex = 0;

    if (CountSubstring(Token, "/") == 0)
    {
        *VertexIndex = stoi(Token) - 1;
    }
    else if (CountSubstring(Token, "/") == 1)
    {
        istringstream subtokens(Token);
        getline(subtokens, vertexString, '/');
        getline(subtokens, texcoordString, '/');
        *VertexIndex = stoi(vertexString) - 1;
        *TexcoordIndex = stoi(texcoordString) - 1;
    }
    else if (CountSubstring(Token, "/") == 2)
    {
        istringstream subtokens(Token);
        getline(subtokens, vertexString, '/');
        getline(subtokens, texcoordString, '/');
        getline(subtokens, normalString, '/');
        *VertexIndex = stoi(vertexString) - 1;
        *TexcoordIndex = stoi(texcoordString) - 1;
        *NormalIndex = stoi(normalString) - 1;
    }
}

int MuObjModel::CountSubstring(string str, string sub)
{
    int count = 0;
    for (int i = 0; (i = str.find(sub, i)) != string::npos; i++)
    {
        count++;
    }

    return count;
}
