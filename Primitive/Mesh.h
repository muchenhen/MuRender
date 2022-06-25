#pragma once
#include <vector>
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