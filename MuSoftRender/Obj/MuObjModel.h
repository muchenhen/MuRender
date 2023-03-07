#pragma once

#include <string>
#include <vector>
#include "../Math/MuMath.h"
#include "../Function/MuStruct.h"

using namespace std;

class MuObjModel
{
public:
    bool Load(const string& Filename);

private:
    // 顶点坐标
    vector<MuPoint3F> Vertices;
    // 法线
    vector<MuVector3F> Normals;
    // 纹理坐标
    vector<MuPoint2F> Texcoords;
    // 顶点索引
    vector<int> Indices;

    void ParseVertexIndex(string Token, int* VertexIndex, int* TexcoordIndex, int* NormalIndex);

    int CountSubstring(string str, string sub);
};
