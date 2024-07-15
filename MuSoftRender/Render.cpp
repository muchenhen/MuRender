#include "Render.h"
#include <algorithm>
#include <cmath>

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

void Renderer::DrawPixel(int x, int y, unsigned int color)
{
    if (x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight)
    {
        FrameBuffer[y * ScreenWidth + x] = color;
    }
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2, unsigned int color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void Renderer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int color)
{
    // 按y坐标排序顶点
    if (y1 > y2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    if (y1 > y3)
    {
        std::swap(x1, x3);
        std::swap(y1, y3);
    }
    if (y2 > y3)
    {
        std::swap(x2, x3);
        std::swap(y2, y3);
    }

    int totalHeight = y3 - y1;
    for (int y = y1; y <= y3; y++)
    {
        bool secondHalf = y > y2 || y2 == y1;
        int segmentHeight = secondHalf ? y3 - y2 : y2 - y1;
        float alpha = static_cast<float>(y - y1) / totalHeight;
        float beta = static_cast<float>(y - (secondHalf ? y2 : y1)) / segmentHeight;

        int Ax = x1 + (x3 - x1) * alpha;
        int Bx = secondHalf ? x2 + (x3 - x2) * beta : x1 + (x2 - x1) * beta;

        if (Ax > Bx) std::swap(Ax, Bx);

        for (int x = Ax; x <= Bx; x++)
        {
            DrawPixel(x, y, color);
        }
    }
}