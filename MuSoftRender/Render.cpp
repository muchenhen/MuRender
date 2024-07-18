#include "Render.h"
#include <algorithm>
#include <cmath>

#include "Cube.h"

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

void Renderer::RenderCamera(const Scene& scene, const Camera& camera)
{
    Eigen::Matrix4f viewMatrix = camera.GetViewMatrix();
    Eigen::Matrix4f projectionMatrix = camera.GetProjectionMatrix();
    Eigen::Matrix4f viewProjectionMatrix = projectionMatrix * viewMatrix;

    for (const auto& object : scene.GetObjects())
    {
        Eigen::Matrix4f modelMatrix = object->GetModelMatrix();
        Eigen::Matrix4f mvpMatrix = viewProjectionMatrix * modelMatrix;

        if (const Cube* cube = dynamic_cast<const Cube*>(object.get()))
        {
            auto mesh = cube->GetMesh();
            if (!mesh) continue;

            for (size_t i = 0; i < mesh->indices.size(); i += 3)
            {
                Eigen::Vector4f v0 = mvpMatrix * mesh->vertices[mesh->indices[i]].position.homogeneous();
                Eigen::Vector4f v1 = mvpMatrix * mesh->vertices[mesh->indices[i + 1]].position.homogeneous();
                Eigen::Vector4f v2 = mvpMatrix * mesh->vertices[mesh->indices[i + 2]].position.homogeneous();

                // 执行透视除法
                v0 /= v0.w();
                v1 /= v1.w();
                v2 /= v2.w();

                // 视口变换
                int x0 = static_cast<int>((v0.x() + 1.0f) * 0.5f * ScreenWidth);
                int y0 = static_cast<int>((1.0f - v0.y()) * 0.5f * ScreenHeight);
                int x1 = static_cast<int>((v1.x() + 1.0f) * 0.5f * ScreenWidth);
                int y1 = static_cast<int>((1.0f - v1.y()) * 0.5f * ScreenHeight);
                int x2 = static_cast<int>((v2.x() + 1.0f) * 0.5f * ScreenWidth);
                int y2 = static_cast<int>((1.0f - v2.y()) * 0.5f * ScreenHeight);

                // 绘制三角形边框
                DrawLine(x0, y0, x1, y1, 0xFFFFFF);
                DrawLine(x1, y1, x2, y2, 0xFFFFFF);
                DrawLine(x2, y2, x0, y0, 0xFFFFFF);
            }
        }
    }
}