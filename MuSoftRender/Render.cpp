#include "Render.h"
#include <algorithm>
#include <cmath>
#include <iostream>

#include "Cube.h"
#include "Logger.h"

Renderer::Renderer(int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;
    FrameBuffer.resize(width * height);
    DepthBuffer.resize(width * height);
}

void Renderer::Clear(uint32_t color)
{
    std::fill(FrameBuffer.begin(), FrameBuffer.end(), color);
    std::fill(DepthBuffer.begin(), DepthBuffer.end(), 1.0f);
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

void Renderer::DrawPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight)
    {
        FrameBuffer[y * ScreenWidth + x] = color;
    }
}

void Renderer::DrawPixel(int x, int y, float depth, uint32_t color)
{
    if (x >= 0 && x < ScreenWidth && y >= 0 && y < ScreenHeight)
    {
        int index = y * ScreenWidth + x;
        if (depth < DepthBuffer[index])
        {
            FrameBuffer[index] = color;
            DepthBuffer[index] = depth;
        }
    }
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2, uint32_t color)
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

void Renderer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
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

void Renderer::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
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
        float alpha = (float)(y - y1) / totalHeight;
        float beta = (float)(y - (secondHalf ? y2 : y1)) / segmentHeight;

        int xa = x1 + (x3 - x1) * alpha;
        int xb = secondHalf ? x2 + (x3 - x2) * beta : x1 + (x2 - x1) * beta;

        if (xa > xb) std::swap(xa, xb);

        for (int x = xa; x <= xb; x++)
        {
            DrawPixel(x, y, color);
        }
    }
}

void Renderer::FillTriangle(
    int x1, int y1, float z1, uint32_t color1,
    int x2, int y2, float z2, uint32_t color2,
    int x3, int y3, float z3, uint32_t color3)
{
    // 按y坐标排序顶点
    if (y1 > y2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
        std::swap(z1, z2);
        std::swap(color1, color2);
    }
    if (y1 > y3)
    {
        std::swap(x1, x3);
        std::swap(y1, y3);
        std::swap(z1, z3);
        std::swap(color1, color3);
    }
    if (y2 > y3)
    {
        std::swap(x2, x3);
        std::swap(y2, y3);
        std::swap(z2, z3);
        std::swap(color2, color3);
    }

    int totalHeight = y3 - y1;
    for (int y = y1; y <= y3; y++)
    {
        bool secondHalf = y > y2 || y2 == y1;
        int segmentHeight = secondHalf ? y3 - y2 : y2 - y1;
        float alpha = (float)(y - y1) / totalHeight;
        float beta = (float)(y - (secondHalf ? y2 : y1)) / segmentHeight;

        int xa = x1 + (x3 - x1) * alpha;
        int xb = secondHalf ? x2 + (x3 - x2) * beta : x1 + (x2 - x1) * beta;

        float za = z1 + (z3 - z1) * alpha;
        float zb = secondHalf ? z2 + (z3 - z2) * beta : z1 + (z2 - z1) * beta;

        auto InterpolateColor = [](uint32_t color1, uint32_t color2, float alpha)
        {
            uint8_t r1 = (color1 >> 16) & 0xFF, r2 = (color2 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF, g2 = (color2 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF, b2 = color2 & 0xFF;

            uint8_t r = r1 + (r2 - r1) * alpha;
            uint8_t g = g1 + (g2 - g1) * alpha;
            uint8_t b = b1 + (b2 - b1) * alpha;

            return (r << 16) | (g << 8) | b;
        };

        uint32_t colorA = InterpolateColor(color1, color3, alpha);
        uint32_t colorB = secondHalf ? InterpolateColor(color2, color3, beta) : InterpolateColor(color1, color2, beta);

        if (xa > xb)
        {
            std::swap(xa, xb);
            std::swap(za, zb);
            std::swap(colorA, colorB);
        }

        for (int x = xa; x <= xb; x++)
        {
            float phi = (xa == xb) ? 1.0 : (float)(x - xa) / (float)(xb - xa);
            float z = za + (zb - za) * phi;
            uint32_t color = InterpolateColor(colorA, colorB, phi);
            DrawPixel(x, y, z, color);
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
        const MeshObject* meshObject = dynamic_cast<const MeshObject*>(object.get());
        if (!meshObject) continue;

        Eigen::Matrix4f modelMatrix = object->GetModelMatrix();
        Eigen::Matrix4f mvpMatrix = viewProjectionMatrix * modelMatrix;
        if (mvpMatrix.array().isNaN().any())
        {
            std::cerr << "Warning: MVP matrix contains NaN values!" << std::endl;
            LOG_ERROR("Warning: MVP matrix contains NaN values!");
            LOG_ERROR("View Matrix:\n", viewMatrix);
            LOG_ERROR("Projection Matrix:\n", projectionMatrix);
            LOG_ERROR("View Projection Matrix:\n", viewProjectionMatrix);
            LOG_ERROR("Model Matrix:\n", modelMatrix);
            LOG_ERROR("MVP Matrix:\n", mvpMatrix);
        }

        const Mesh* mesh = meshObject->GetMesh();
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

            float z0 = v0.z();
            float z1 = v1.z();
            float z2 = v2.z();

            // 绘制三角形边框
            /*DrawLine(x0, y0, x1, y1, 0xFFFFFF);
            DrawLine(x1, y1, x2, y2, 0xFFFFFF);
            DrawLine(x2, y2, x0, y0, 0xFFFFFF);*/

            // FillTriangle(x0, y0, x1, y1, x2, y2, 0xFFFFFF);

            FillTriangle(x0, y0, z0, 0xFFFFFF, x1, y1, z1, 0xFFFFFF, x2, y2, z2, 0xFFFFFF);

             //DrawLine(x0, y0, x1, y1, 0xFF0000);
             //DrawLine(x1, y1, x2, y2, 0xFF0000);
             //DrawLine(x2, y2, x0, y0, 0xFF0000);
        }
    }
}