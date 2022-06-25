#include "Model.h"

#include "Colors.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::CreateTriangle3D()
{
    Mesh* meshTri = new Mesh();
    Vertex v[5];
    v[0] = Vertex(MuVector(0.0f, 1.0f, 0.0f, 1.0f), ColorsL::Green, MuVector(0.5f, 0.0f, 0.0f, 0.0f));
    v[1] = Vertex(MuVector(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[2] = Vertex(MuVector(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Magenta, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    v[3] = Vertex(MuVector(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Red, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[4] = Vertex(MuVector(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Azure, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    unsigned int ar[18] =
        {
            0, 1, 2,
            0, 3, 4,
            0, 4, 1,
            0, 2, 3,
            1, 3, 2,
            1, 4, 3
        };
    meshTri->vertexBuffer.assign(v, v + 5);
    meshTri->indexBuffer.assign(ar, ar + 18);
    ModelList.push_back(meshTri);
}

void Model::CreateBox3D()
{
    Mesh* meshBox = new Mesh();
    Vertex v[24];
    v[0] = Vertex(MuVector(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[1] = Vertex(MuVector(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, MuVector(0.0f, 0.0f, 0.0f, 0.0f));
    v[2] = Vertex(MuVector(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Cyan, MuVector(1.0f, 0.0f, 0.0f, 0.0f));
    v[3] = Vertex(MuVector(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Magenta, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    v[4] = Vertex(MuVector(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Lavender, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    v[5] = Vertex(MuVector(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[6] = Vertex(MuVector(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, MuVector(0.0f, 0.0f, 0.0f, 0.0f));
    v[7] = Vertex(MuVector(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Green, MuVector(1.0f, 0.0f, 0.0f, 0.0f));
    v[8] = Vertex(MuVector(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Magenta, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[9] = Vertex(MuVector(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Cyan, MuVector(0.0f, 0.0f, 0.0f, 0.0f));
    v[10] = Vertex(MuVector(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, MuVector(1.0f, 0.0f, 0.0f, 0.0f));
    v[11] = Vertex(MuVector(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Green, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    v[12] = Vertex(MuVector(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Blue, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    v[13] = Vertex(MuVector(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[14] = Vertex(MuVector(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, MuVector(0.0f, 0.0f, 0.0f, 0.0f));
    v[15] = Vertex(MuVector(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Lavender, MuVector(1.0f, 0.0f, 0.0f, 0.0f));
    v[16] = Vertex(MuVector(-1.0f, -1.0f, 1.0f, 1.0f), ColorsL::Magenta, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[17] = Vertex(MuVector(-1.0f, 1.0f, 1.0f, 1.0f), ColorsL::Blue, MuVector(0.0f, 0.0f, 0.0f, 0.0f));
    v[18] = Vertex(MuVector(-1.0f, 1.0f, -1.0f, 1.0f), ColorsL::Lavender, MuVector(1.0f, 0.0f, 0.0f, 0.0f));
    v[19] = Vertex(MuVector(-1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Green, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    v[20] = Vertex(MuVector(1.0f, -1.0f, -1.0f, 1.0f), ColorsL::Cyan, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
    v[21] = Vertex(MuVector(1.0f, 1.0f, -1.0f, 1.0f), ColorsL::YellowGreen, MuVector(0.0f, 0.0f, 0.0f, 0.0f));
    v[22] = Vertex(MuVector(1.0f, 1.0f, 1.0f, 1.0f), ColorsL::DarkSlateGray, MuVector(1.0f, 0.0f, 0.0f, 0.0f));
    v[23] = Vertex(MuVector(1.0f, -1.0f, 1.0f, 1.0f), ColorsL::DarkOrange, MuVector(1.0f, 1.0f, 0.0f, 0.0f));
    for (auto& i : v)
    {
        i.vertexPosChange(1.0f);
    }

    unsigned int ar[36] =
        {
            0, 2, 1,
            0, 3, 2,
            4, 6, 5,
            4, 7, 6,
            8, 10, 9,
            8, 11, 10,
            12, 14, 13,
            12, 15, 14,
            16, 18, 17,
            16, 19, 18,
            20, 22, 21,
            20, 23, 22};
    meshBox->vertexBuffer.assign(v, v + 24);
    meshBox->indexBuffer.assign(ar, ar + 36);
    ModelList.push_back(meshBox);
}

void Model::ObjCube()
{
    Obj obj;
    obj.VertexPositionList.emplace_back(-1.0f, -1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, -1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, 1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(-1.0f, 1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(-1.0f, -1.0f, 1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, -1.0f, 1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(-1.0f, 1.0f, 1.0f, 1.0f);
    obj.UVPositionList.emplace_back(0.0f, 0.0f);
    obj.UVPositionList.emplace_back(0.0f, 1.0f);
    obj.UVPositionList.emplace_back(1.0f, 0.0f);
    obj.UVPositionList.emplace_back(1.0f, 1.0f);
    obj.FaceIndexPos = {
        0, 2, 1, 0, 3, 2, 4, 7, 6, 4, 6, 5, 0, 7, 4, 0, 7, 3, 1, 6, 5, 1, 6, 2, 2, 7, 6, 2, 7, 3, 0, 5, 1, 0, 5, 4};

    obj.FaceIndexUV = {
        0, 2, 1, 0, 3, 2,
        0, 3, 2, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 1, 0, 2, 3};

    for (int i = 0; i < obj.FaceIndexPos.size(); i++)
    {
        obj.VertexList.emplace_back(obj.VertexPositionList[obj.FaceIndexPos[i]], ColorsL::AliceBlue, obj.UVPositionList[obj.FaceIndexUV[i]]);
    }

    Mesh* mesh = new Mesh();
    mesh->vertexBuffer = obj.VertexList;
    for (int i = 0; i < mesh->vertexBuffer.size(); i++)
    {
        mesh->indexBuffer.push_back(i);
    }
    ModelList.push_back(mesh);
}

void Model::ObjCube2()
{
    Obj obj;
    obj.VertexPositionList.emplace_back(-1.0f, -1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(-1.0f, 1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, 1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, -1.0f, -1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(-1.0f, -1.0f, 1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, -1.0f, 1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(1.0f, 1.0f, 1.0f, 1.0f);
    obj.VertexPositionList.emplace_back(-1.0f, 1.0f, 1.0f, 1.0f);
    obj.UVPositionList.emplace_back(0.0f, 0.0f);
    obj.UVPositionList.emplace_back(0.0f, 1.0f);
    obj.UVPositionList.emplace_back(1.0f, 0.0f);
    obj.UVPositionList.emplace_back(1.0f, 1.0f);
    obj.FaceIndexPos = {
        0, 2, 1, 0, 3, 2,
        4, 6, 5, 4, 7, 6,
        4, 1, 7, 4, 0, 1,
        3, 6, 2, 3, 5, 6,
        1, 6, 7, 1, 2, 6,
        0, 5, 3, 0, 4, 5};

    obj.FaceIndexUV = {
        0, 2, 1, 0, 3, 2,
        0, 3, 2, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 3, 0, 2, 1,
        0, 2, 1, 0, 2, 3};

    for (int i = 0; i < obj.FaceIndexPos.size(); i++)
    {
        obj.VertexList.emplace_back(obj.VertexPositionList[obj.FaceIndexPos[i]], ColorsL::AliceBlue, obj.UVPositionList[obj.FaceIndexUV[i]]);
    }

    Mesh* mesh = new Mesh();
    mesh->vertexBuffer = obj.VertexList;
    for (int i = 0; i < mesh->vertexBuffer.size(); i++)
    {
        mesh->indexBuffer.push_back(i);
    }
    ModelList.push_back(mesh);
}

void Model::ObjBuffer(const string Filename)
{
    Obj obj;
    obj.LoadObj(Filename);
    Mesh* mesh = new Mesh();

    for (int i = 0; i < obj.FaceIndexPos.size(); i++)
    {
        obj.VertexList.emplace_back(obj.VertexPositionList[obj.FaceIndexPos[i]], randColor(), obj.UVPositionList[obj.FaceIndexUV[i]]);
    }

    mesh->vertexBuffer = obj.VertexList;
    for (int i = 0; i < mesh->vertexBuffer.size(); i++)
    {
        mesh->indexBuffer.push_back(i);
    }

    ModelList.push_back(mesh);
}
