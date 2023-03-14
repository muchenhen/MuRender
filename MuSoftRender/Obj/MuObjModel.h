#pragma once

#include <string>
#include <vector>
#include "../Math/MuMath.h"
#include "../Function/MuStruct.h"
#include "../TGA/TGAImage.h"

using namespace std;

class MuObjModel
{
public:
    bool Load(const string& Filename);
    int GetFaceCount() const;
    FMuObjFace GetFace(int I) const;
    MuPoint3F GetVertexByIndex(int VertexIndex);
    MuPoint2F GetTexcoordByIndex(int TexcoordIndex);
    vector<MuPoint3F> GetAllVertices();
    bool LoadTexture(const string& Filename);
    TGAImage* GetTexture() const;
    
private:
    // 顶点坐标
    vector<MuPoint3F> Vertices;
    // 法线
    vector<MuVector3F> Normals;
    // 纹理坐标
    vector<MuPoint2F> Texcoords;
    // 顶点索引
    vector<FMuObjFace> FaceIndices;
    // 贴图
    TGAImage* Texture;

    void ParseVertexIndex(string Token, int* VertexIndex, int* TexcoordIndex, int* NormalIndex);

    int CountSubstring(string str, string sub);

};

inline TGAImage* MuObjModel::GetTexture() const
{
    return Texture;
}
