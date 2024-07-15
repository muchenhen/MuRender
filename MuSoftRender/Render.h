#pragma once

#include <vector>
#include <cstdint>

#define UINT_32 unsigned int

class Renderer
{
private:
    int ScreenWidth;
    int ScreenHeight;
    std::vector<uint32_t> FrameBuffer;

public:
    Renderer(int width, int height);

    void Clear(uint32_t color = 0);

    const std::vector<uint32_t>& GetFrameBuffer() const;

    int GetWidth() const;

    int GetHeight() const;


public:
    void DrawPixel(int x, int y, unsigned int color);

    void DrawLine(int x1, int y1, int x2, int y2, unsigned int color);

    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int color);
};
