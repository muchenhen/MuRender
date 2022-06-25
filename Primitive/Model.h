
#pragma once
#include <vector>
#include "Mesh.h"
#include "Obj.h"
using namespace std;

class Model
{
public:
    vector<Mesh*> ModelList;

    Model();
    ~Model();

    void CreateTriangle3D();
    void CreateBox3D();

    void ObjCube();
    void ObjCube2();

    void ObjBuffer(string Filename);
};
