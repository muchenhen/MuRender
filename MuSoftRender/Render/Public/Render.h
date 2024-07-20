#pragma once

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
    Renderer(int Width, int Height);

    void Clear(uint32_t Color = 0);

    const std::vector<uint32_t>& GetFrameBuffer() const;

    int GetWidth() const;

    int GetHeight() const;

private:
    Eigen::Vector3f ComputeBarycentric(int X, int Y, int X0, int Y0, int X1, int Y1, int X2, int Y2);

    bool IsInsideTriangle(int X, int Y, int X0, int Y0, int X1, int Y1, int X2, int Y2);

    uint32_t ColorToUint32(const Eigen::Vector3f& Color);

public:
    void DrawPixel(int X, int Y, uint32_t Color);

    void DrawPixel(int X, int Y, float Depth, uint32_t Color);

    void DrawLine(int X1, int Y1, int X2, int Y2, uint32_t Color);

    void DrawTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, uint32_t Color);

    void FillTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, uint32_t Color);

    void FillTriangle(int X0, int Y0, float Z0, uint32_t Color0,
                      int X1, int Y1, float Z1, uint32_t Color1,
                      int X2, int Y2, float Z2, uint32_t Color2);

public:
    void RenderCamera(const Scene& Scene, const Camera& Camera);
};