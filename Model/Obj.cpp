#include "Obj.h"
#include <fstream>
#include <sstream>
#include "Colors.h"

void Obj::CreateCubeObj()
{
    VertexPositionList.emplace_back(-1.0f, -1.0f, -1.0f, 1.0f);
    VertexPositionList.emplace_back(1.0f, -1.0f, -1.0f, 1.0f);
    VertexPositionList.emplace_back(1.0f, 1.0f, -1.0f, 1.0f);
    VertexPositionList.emplace_back(-1.0f, 1.0f, -1.0f, 1.0f);
    VertexPositionList.emplace_back(-1.0f, -1.0f, 1.0f, 1.0f);
    VertexPositionList.emplace_back(1.0f, -1.0f, 1.0f, 1.0f);
    VertexPositionList.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
    VertexPositionList.emplace_back(-1.0f, 1.0f, 1.0f, 1.0f);
    UVPositionList.emplace_back(0.0f, 0.0f);
    UVPositionList.emplace_back(0.0f, 1.0f);
    UVPositionList.emplace_back(1.0f, 0.0f);
    UVPositionList.emplace_back(1.0f, 1.0f);
    FaceIndexPos = {
        0, 2, 1, 0, 3, 2, 4, 7, 6, 4, 6, 5, 0, 7, 4, 0, 7, 3, 1, 6, 5, 1, 6, 2, 2, 7, 6, 2, 7, 3, 0, 5, 1, 0, 5, 4};

    FaceIndexUV = {
        0, 2, 1, 0, 3, 2,
        0, 3, 2, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 1, 0, 2, 3};

    for (int i = 0; i < FaceIndexPos.size(); i++)
    {
        VertexList.emplace_back(VertexPositionList[FaceIndexPos[i]], ColorsL::AliceBlue, UVPositionList[FaceIndexUV[i]]);
    }
}

bool Obj::LoadObj(const string& Filename)
{
    ifstream Fin;
    Fin.open(Filename, ios_base::in);

    if (!Fin)
        return false;

    string flag;
    char line[100];
    stringstream Ss;

    Fin.getline(line, 100);

    while (Fin.peek() != EOF)
    {
        Ss.clear();
        Ss.str(line);
        Ss >> flag;

        if (flag == "v")
        {
            MuVector3D v;
            Ss >> v.X >> v.Y >> v.Z;
            VertexPositionList.emplace_back(v);
        }
        else if (flag == "vt")
        {
            MuVector3D Vn;
            Ss >> Vn.X >> Vn.Y >> Vn.Z;
            UVPositionList.emplace_back(Vn);
        }
        else if (flag == "f")
        {
            char s1[20], s2[20], s3[20];
            Ss >> s1 >> s2 >> s3;

            vector<string> tr1 = Split(s1, "/");
            vector<string> tr2 = Split(s2, "/");
            vector<string> tr3 = Split(s3, "/");

            FaceIndexPos.push_back(stoi(tr1[0]) - 1);
            FaceIndexPos.push_back(stoi(tr2[0]) - 1);
            FaceIndexPos.push_back(stoi(tr3[0]) - 1);

            FaceIndexUV.push_back(stoi(tr1[1]) - 1);
            FaceIndexUV.push_back(stoi(tr2[1]) - 1);
            FaceIndexUV.push_back(stoi(tr3[1]) - 1);
        }
        Fin.getline(line, 100);
    }
    Fin.close();

    return true;
}

vector<string> Obj::Split(char* Data, const char* D)
{
    vector<string> Strings;
    char* Buff = nullptr;
    const char* P = strtok_s(Data, D, &Buff);
    while (P)
    {
        Strings.emplace_back(P);
        P = strtok_s(nullptr, D, &Buff);
    }
    return Strings;
}