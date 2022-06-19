
#pragma once
#include <vector>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <istream>
#include <sstream>
#include "Vector.h"
#include "Mesh.h"
#include "Colors.h"
#include "Obj.h"
using namespace std;

class Model
{
public:
	vector<Mesh*> ModelList;

	Model();
	~Model();

	//创建三角形mesh
	void creatTriangle3D();
	//创建box mesh
	void creatBox3D();

	void objCube();
	void objCube2();

	void objBuffer(string filename);
};
