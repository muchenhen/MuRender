#pragma once
#include <vector>
#include <windows.h>


#include "Vector.h"
#include "Vertex.h"
using namespace std;

class Mesh
{
public:

	vector<unsigned int> indexBuffer;
	vector<Vertex> vertexBuffer;

	Mesh();
	~Mesh();
};