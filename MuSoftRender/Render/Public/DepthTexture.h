#pragma once
#include <vector>

class DepthTexture
{
public:
    std::vector<float> Data;
    int Width;
    int Height;

    DepthTexture(int InWidth, int InHeight);

    void SetDepth(int x, int y, float InDepth);

    float GetDepth(int x, int y) const;
};
