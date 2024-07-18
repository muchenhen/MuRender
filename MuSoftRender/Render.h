﻿#pragma once

#include <vector>
#include <cstdint>

#include "Camera.h"
#include "Scene.h"

class Renderer
{
private:
    int ScreenWidth;
    int ScreenHeight;
    std::vector<uint32_t> FrameBuffer;
    std::vector<float> DepthBuffer;

public:
    Renderer(int width, int height);

    void Clear(uint32_t color = 0);

    const std::vector<uint32_t>& GetFrameBuffer() const;

    int GetWidth() const;

    int GetHeight() const;


public:
    void DrawPixel(int x, int y, uint32_t color);

    void DrawPixel(int x, int y, float depth, uint32_t color);

    void DrawLine(int x1, int y1, int x2, int y2, uint32_t color);

    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);

    void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);

    void FillTriangle(int x1, int y1, float z1, uint32_t color1,
                      int x2, int y2, float z2, uint32_t color2,
                      int x3, int y3, float z3, uint32_t color3);

public:
    void RenderCamera(const Scene& Scene, const Camera& Camera);
};
