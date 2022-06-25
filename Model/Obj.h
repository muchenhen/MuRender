/*
读取Obj格式
*/

#pragma once
#include <string>
#include <istream>
#include <vector>
#include "MathUnion.h"
#include "Vertex.h"
#include "Mesh.h"
using namespace std;
class Obj
{
public:
    vector<MuVector> VertexPositionList;
    vector<MuVector> UVPositionList;
    vector<MuVector> VertexNormalVectorList;

    vector<unsigned int> FaceIndexPos;
    vector<unsigned int> FaceIndexUV;
    vector<unsigned int> FaceIndexVN;

    vector<Vertex> VertexList;

    Mesh* Mesh;

    Obj() = default;
    ~Obj() = default;

    void CreateCubeObj();

    bool LoadObj(const string& Filename);

    static vector<string> Split(char* Data, const char* D);
};
