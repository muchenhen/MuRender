#pragma once
#include <vector>
#include <fstream>
#include <cstdint>
#include <algorithm>

class DepthTexture
{
public:
    static constexpr float INVALID_DEPTH = 2.0f; // 使用大于1的值表示无效深度
    
    std::vector<float> DepthBuffer;
    int Width;
    int Height;

    DepthTexture(int InWidth, int InHeight);

    void Clear();

    void SetDepth(int x, int y, float InDepth);

    float GetDepth(int x, int y) const;

    int GetWidth() const;

    int GetHeight() const;
    void RotateDepthBuffer90DegreesCounterClockwise();

    static void SaveDepthTextureToBMP(DepthTexture* depthTexture, const std::string& filename);

    float SampleDepth(float u, float v) const;
};