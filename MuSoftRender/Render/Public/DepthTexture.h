#pragma once
#include <vector>
#include <fstream>
#include <cstdint>
#include <algorithm>

class DepthTexture
{
public:
    std::vector<float> Data;
    int Width;
    int Height;

    DepthTexture(int InWidth, int InHeight);

    void SetDepth(int x, int y, float InDepth);

    float GetDepth(int x, int y) const;

    int GetWidth() const;

    int GetHeight() const;

    static void SaveDepthTextureToBMP(const DepthTexture* depthTexture, const std::string& filename);

    float SampleDepth(float u, float v) const;
};