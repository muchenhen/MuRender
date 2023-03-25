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
    MuObjModel();
    ~MuObjModel();
    
    bool Load(const string& Filename);
    bool LoadTexture(const string& Filename);

    int GetFaceCount() const;
    int GetVertexCount() const;
    FMuObjFace GetFace(int I) const;
    MuPoint3F GetVertexByIndex(int VertexIndex);
    MuPoint3F GetScreenVertexByIndex(int VertexIndex);
    MuPoint2F GetTexcoordByIndex(int TexcoordIndex);
    vector<MuPoint3F> GetAllVertices();
    TGAImage* GetTexture() const;
    void SetScreenVertex(const vector<MuPoint3F>& ScreenVertices);
    
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
    TGAImage* Texture = nullptr;

    // 当前屏幕空间顶点坐标
    vector<MuPoint3F> ScreenVertices;
    
    void ParseVertexIndex(string Token, int* VertexIndex, int* TexcoordIndex, int* NormalIndex);

    int CountSubstring(string str, string sub);

};

inline TGAImage* MuObjModel::GetTexture() const
{
    return Texture;
}
