#include "DrawFunction.h"
#include <algorithm>
#include <cmath>

void DrawFunctions::DrawPixel(const int& x, const int& y, unsigned int color)
{
}

void DrawFunctions::DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, unsigned int color)
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

void DrawFunctions::DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, unsigned int color)
{
    // °´y×ø±êÅÅÐò¶¥µã
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