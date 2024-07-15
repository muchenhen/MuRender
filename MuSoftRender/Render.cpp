#include "Render.h"

Renderer::Renderer(int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;
    FrameBuffer.resize(width * height);
}

void Renderer::Clear(uint32_t color)
{
    std::fill(FrameBuffer.begin(), FrameBuffer.end(), color);
}

const std::vector<uint32_t>& Renderer::GetFrameBuffer() const
{
    return FrameBuffer;
}

int Renderer::GetWidth() const
{
    return ScreenWidth;
}

int Renderer::GetHeight() const
{
    return ScreenHeight;
}