#pragma once
#define uint_32 unsigned int

class DrawFunctions
{
public:
    static void DrawPixel(const int& x, const int& y, uint_32 color);
    static void DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, uint_32 color);
    static void DrawTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, uint_32 color);
};