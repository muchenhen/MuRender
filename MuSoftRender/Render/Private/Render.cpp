#include "Render.h"
#include <algorithm>
#include <cmath>
#include <iostream>

#include "Cube.h"
#include "Logger.h"

Renderer::Renderer(const int Width, const int Height)
{
    ScreenWidth = Width;
    ScreenHeight = Height;
    FrameBuffer.resize(static_cast<size_t>(Width) * static_cast<size_t>(Height));
    DepthBuffer.resize(static_cast<size_t>(Width) * static_cast<size_t>(Height));
}

void Renderer::Clear(const uint32_t Color)
{
    std::fill(FrameBuffer.begin(), FrameBuffer.end(), Color);
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

void Renderer::DrawPixel(const int X, const int Y, const uint32_t Color)
{
    if (X >= 0 && X < ScreenWidth && Y >= 0 && Y < ScreenHeight)
    {
        FrameBuffer[Y * ScreenWidth + X] = Color;
    }
}

void Renderer::DrawPixel(const int X, const int Y, const float Depth, const uint32_t Color)
{
    if (X >= 0 && X < ScreenWidth && Y >= 0 && Y < ScreenHeight)
    {
        const int Index = Y * ScreenWidth + X;
        if (Depth < DepthBuffer[Index])
        {
            FrameBuffer[Index] = Color;
            DepthBuffer[Index] = Depth;
        }
    }
}

void Renderer::DrawLine(int X1, int Y1, const int X2, const int Y2, const uint32_t Color)
{
    const int Dx = abs(X2 - X1);
    const int Dy = abs(Y2 - Y1);
    const int Sx = (X1 < X2) ? 1 : -1;
    const int Sy = (Y1 < Y2) ? 1 : -1;
    int Err = Dx - Dy;

    while (true)
    {
        DrawPixel(X1, Y1, Color);
        if (X1 == X2 && Y1 == Y2) break;
        const int E2 = 2 * Err;
        if (E2 > -Dy)
        {
            Err -= Dy;
            X1 += Sx;
        }
        if (E2 < Dx)
        {
            Err += Dx;
            Y1 += Sy;
        }
    }
}

void Renderer::DrawTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, const uint32_t Color)
{
    // 按y坐标排序顶点
    if (Y1 > Y2)
    {
        std::swap(X1, X2);
        std::swap(Y1, Y2);
    }
    if (Y1 > Y3)
    {
        std::swap(X1, X3);
        std::swap(Y1, Y3);
    }
    if (Y2 > Y3)
    {
        std::swap(X2, X3);
        std::swap(Y2, Y3);
    }

    const int TotalHeight = Y3 - Y1;
    for (int Y = Y1; Y <= Y3; Y++)
    {
        const bool SecondHalf = Y > Y2 || Y2 == Y1;
        const int SegmentHeight = SecondHalf ? Y3 - Y2 : Y2 - Y1;
        const float Alpha = static_cast<float>(Y - Y1) / static_cast<float>(TotalHeight);
        const float Beta = static_cast<float>(Y - (SecondHalf ? Y2 : Y1)) /  static_cast<float>(SegmentHeight);

        int Ax = X1 + static_cast<int>(static_cast<float>(X3 - X1) * Alpha);
        int Bx = SecondHalf ? X2 + static_cast<int>(static_cast<float>(X3 - X2) * Beta) : X1 + static_cast<int>(static_cast<float>(X2 - X1) * Beta);

        if (Ax > Bx) std::swap(Ax, Bx);

        for (int X = Ax; X <= Bx; X++)
        {
            DrawPixel(X, Y, Color);
        }
    }
}

void Renderer::FillTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, const uint32_t Color)
{
    // 按y坐标排序顶点
    if (Y1 > Y2)
    {
        std::swap(X1, X2);
        std::swap(Y1, Y2);
    }
    if (Y1 > Y3)
    {
        std::swap(X1, X3);
        std::swap(Y1, Y3);
    }
    if (Y2 > Y3)
    {
        std::swap(X2, X3);
        std::swap(Y2, Y3);
    }

    const int TotalHeight = Y3 - Y1;
    for (int Y = Y1; Y <= Y3; Y++)
    {
        const bool SecondHalf = Y > Y2 || Y2 == Y1;
        const int SegmentHeight = SecondHalf ? Y3 - Y2 : Y2 - Y1;
        const float Alpha = static_cast<float>(Y - Y1) / static_cast<float>(TotalHeight);
        const float Beta = static_cast<float>(Y - (SecondHalf ? Y2 : Y1)) / static_cast<float>(SegmentHeight);

        int Ax = X1 + static_cast<int>(static_cast<float>(X3 - X1) * Alpha);
        int Bx = SecondHalf ? X2 + static_cast<int>(static_cast<float>(X3 - X2) * Beta) : X1 + static_cast<int>(static_cast<float>(X2 - X1) * Beta);

        if (Ax > Bx) std::swap(Ax, Bx);

        for (int X = Ax; X <= Bx; X++)
        {
            DrawPixel(X, Y, Color);
        }
    }
}

void Renderer::FillTriangle(
    int X1, int Y1, float Z1, uint32_t Color1,
    int X2, int Y2, float Z2, uint32_t Color2,
    int X3, int Y3, float Z3, uint32_t Color3)
{
    // 按y坐标排序顶点
    if (Y1 > Y2)
    {
        std::swap(X1, X2);
        std::swap(Y1, Y2);
        std::swap(Z1, Z2);
        std::swap(Color1, Color2);
    }
    if (Y1 > Y3)
    {
        std::swap(X1, X3);
        std::swap(Y1, Y3);
        std::swap(Z1, Z3);
        std::swap(Color1, Color3);
    }
    if (Y2 > Y3)
    {
        std::swap(X2, X3);
        std::swap(Y2, Y3);
        std::swap(Z2, Z3);
        std::swap(Color2, Color3);
    }

    const int TotalHeight = Y3 - Y1;
    for (int Y = Y1; Y <= Y3; Y++)
    {
        const bool SecondHalf = Y > Y2 || Y2 == Y1;
        const int SegmentHeight = SecondHalf ? Y3 - Y2 : Y2 - Y1;
        const float Alpha = static_cast<float>(Y - Y1) / static_cast<float>(TotalHeight);
        const float Beta = static_cast<float>(Y - (SecondHalf ? Y2 : Y1)) / static_cast<float>(SegmentHeight);

        int Ax = X1 + static_cast<int>(static_cast<float>(X3 - X1) * Alpha);
        int Bx = SecondHalf ? X2 + static_cast<int>(static_cast<float>(X3 - X2) * Beta) : X1 + static_cast<int>(static_cast<float>(X2 - X1) * Beta);

        float Az = Z1 + (Z3 - Z1) * Alpha;
        float Bz = SecondHalf ? Z2 + (Z3 - Z2) * Beta : Z1 + (Z2 - Z1) * Beta;

        auto InterpolateColor = [](const uint32_t Color1, const uint32_t Color2, const float Alpha)
        {
            uint8_t R1 = (Color1 >> 16) & 0xFF, R2 = (Color2 >> 16) & 0xFF;
            uint8_t G1 = (Color1 >> 8) & 0xFF, G2 = (Color2 >> 8) & 0xFF;
            uint8_t B1 = Color1 & 0xFF, B2 = Color2 & 0xFF;

            const uint8_t R = R1 + static_cast<uint8_t>(static_cast<float>(R2 - R1) * Alpha);
            const uint8_t G = G1 + static_cast<uint8_t>(static_cast<float>(G2 - G1) * Alpha);
            const uint8_t B = B1 + static_cast<uint8_t>(static_cast<float>(B2 - B1) * Alpha);

            return (R << 16) | (G << 8) | B;
        };

        uint32_t ColorA = InterpolateColor(Color1, Color3, Alpha);
        uint32_t ColorB;
        if (SecondHalf)
            ColorB = InterpolateColor(Color2, Color3, Beta);  // NOLINT(readability-suspicious-call-argument)
        else
            ColorB = InterpolateColor(Color1, Color2, Beta);

        if (Ax > Bx)
        {
            std::swap(Ax, Bx);
            std::swap(Az, Bz);
            std::swap(ColorA, ColorB);
        }

        for (int X = Ax; X <= Bx; X++)
        {
            const float Phi = Ax == Bx ? 1.0f : static_cast<float>(X - Ax) / static_cast<float>(Bx - Ax);
            const float Z = Az + (Bz - Az) * Phi;
            const uint32_t Color = InterpolateColor(ColorA, ColorB, Phi);
            DrawPixel(X, Y, Z, Color);
        }
    }
}

void Renderer::RenderCamera(const Scene& InScene, const Camera& InCamera)
{
    const Eigen::Matrix4f ViewMatrix = InCamera.GetViewMatrix();
    const Eigen::Matrix4f ProjectionMatrix = InCamera.GetProjectionMatrix();
    const Eigen::Matrix4f ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

    for (const auto& Object : InScene.GetObjects())
    {
        const MeshObject* MeshObjectPtr = dynamic_cast<const MeshObject*>(Object.get());
        if (!MeshObjectPtr) continue;

        Eigen::Matrix4f ModelMatrix = Object->GetModelMatrix();
        Eigen::Matrix4f MVPMatrix = ViewProjectionMatrix * ModelMatrix;
        if (MVPMatrix.array().isNaN().any())
        {
            std::cerr << "Warning: MVP matrix contains NaN values!\n";
            LOG_ERROR("Warning: MVP matrix contains NaN values!");
            LOG_ERROR("View Matrix:\n", ViewMatrix);
            LOG_ERROR("Projection Matrix:\n", ProjectionMatrix);
            LOG_ERROR("View Projection Matrix:\n", ViewProjectionMatrix);
            LOG_ERROR("Model Matrix:\n", ModelMatrix);
            LOG_ERROR("MVP Matrix:\n", MVPMatrix);
        }

        const Mesh* MeshPtr = MeshObjectPtr->GetMesh();
        if (!MeshPtr) continue;

        for (size_t i = 0; i < MeshPtr->Indices.size(); i += 3)
        {
            Eigen::Vector4f V0 = MVPMatrix * MeshPtr->Vertices[MeshPtr->Indices[i]].Position.homogeneous();
            Eigen::Vector4f V1 = MVPMatrix * MeshPtr->Vertices[MeshPtr->Indices[i + 1]].Position.homogeneous();
            Eigen::Vector4f V2 = MVPMatrix * MeshPtr->Vertices[MeshPtr->Indices[i + 2]].Position.homogeneous();

            // 执行透视除法
            V0 /= V0.w();
            V1 /= V1.w();
            V2 /= V2.w();

            // 视口变换
            const int X0 = static_cast<int>((V0.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth));
            const int Y0 = static_cast<int>((1.0f - V0.y()) * 0.5f * static_cast<float>(ScreenHeight));
            const int X1 = static_cast<int>((V1.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth));
            const int Y1 = static_cast<int>((1.0f - V1.y()) * 0.5f * static_cast<float>(ScreenHeight));
            const int X2 = static_cast<int>((V2.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth));
            const int Y2 = static_cast<int>((1.0f - V2.y()) * 0.5f * static_cast<float>(ScreenHeight));

            const float Z0 = V0.z();
            const float Z1 = V1.z();
            const float Z2 = V2.z();

            // 绘制三角形边框
            /*DrawLine(x0, y0, x1, y1, 0xFFFFFF);
            DrawLine(x1, y1, x2, y2, 0xFFFFFF);
            DrawLine(x2, y2, x0, y0, 0xFFFFFF);*/

            // FillTriangle(x0, y0, x1, y1, x2, y2, 0xFFFFFF);

            FillTriangle(X0, Y0, Z0, 0xFFFFFF, X1, Y1, Z1, 0xFFFFFF, X2, Y2, Z2, 0xFFFFFF);

            //DrawLine(x0, y0, x1, y1, 0xFF0000);
            //DrawLine(x1, y1, x2, y2, 0xFF0000);
            //DrawLine(x2, y2, x0, y0, 0xFF0000);
        }
    }
}