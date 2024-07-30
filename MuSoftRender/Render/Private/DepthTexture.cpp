#include "DepthTexture.h"

DepthTexture::DepthTexture(int InWidth, int InHeight):
    Width(InWidth), Height(InHeight)
{
    Data.resize(static_cast<size_t>(Width) * Height, 1.0f);
}

void DepthTexture::SetDepth(int x, int y, float InDepth)
{
    if (x >= 0 && x < Width && y >= 0 && y < Height)
    {
        Data[y * Width + x] = InDepth;
    }
}

float DepthTexture::GetDepth(int x, int y) const
{
    if (x >= 0 && x < Width && y >= 0 && y < Height)
    {
        return Data[y * Width + x];
    }
    return 1.0f;
}