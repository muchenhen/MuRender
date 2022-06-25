#pragma once
#include <string>
#include "Vector.h"

class Texture
{
public:
    int width;
    int height;
    unsigned int* data;

    Texture();
    ~Texture();

    bool LoadBMP(const std::string& filename);
    unsigned SampleColor(MuVector& tc);
};