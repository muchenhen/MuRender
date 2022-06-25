#pragma once
#include "Vector.h"

class Vertex
{
public:
    MuVector pos;
    MuVector color;
    MuVector uv;

    Vertex();
    ~Vertex();

    Vertex(const MuVector& p, const MuVector& c);

    Vertex(const MuVector& p, const MuVector& c, const MuVector& tc);

    Vertex(const float& px, const float& py, const float& pz, const MuVector& c, const float pw = 1.0f);

    void vertexPosChange(float a);
};
