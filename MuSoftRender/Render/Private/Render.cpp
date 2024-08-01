#include "Render.h"
#include <algorithm>
#include <cmath>

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
        if (UseEarlyDepthTest)
        {
            FrameBuffer[Index] = Color;
        }
        else
        {
            if (Depth < DepthBuffer[Index])
            {
                DepthBuffer[Index] = Depth;
                FrameBuffer[Index] = Color;
            }
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

void Renderer::DrawLine(int X1, int Y1, const float Z1, const uint32_t Color1, const int X2, const int Y2, const float Z2, uint32_t Color2)
{
    const int Dx = abs(X2 - X1);
    const int Dy = abs(Y2 - Y1);
    const int Sx = (X1 < X2) ? 1 : -1;
    const int Sy = (Y1 < Y2) ? 1 : -1;
    int Err = Dx - Dy;

    while (true)
    {
        float Alpha = static_cast<float>(X1 - X2) / static_cast<float>(X1 - X2 + Y1 - Y2);
        float Z = Z1 + (Z2 - Z1) * Alpha;
        uint32_t Color = Color1;
        DrawPixel(X1, Y1, Z, Color);
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
    int X0, int Y0, float Z0, uint32_t Color0,
    int X1, int Y1, float Z1, uint32_t Color1,
    int X2, int Y2, float Z2, uint32_t Color2)
{
    // 按y坐标排序顶点
    if (Y0 > Y1)
    {
        std::swap(X0, X1);
        std::swap(Y0, Y1);
        std::swap(Z0, Z1);
        std::swap(Color0, Color1);
    }
    if (Y0 > Y2)
    {
        std::swap(X0, X2);
        std::swap(Y0, Y2);
        std::swap(Z0, Z2);
        std::swap(Color0, Color2);
    }
    if (Y1 > Y2)
    {
        std::swap(X1, X2);
        std::swap(Y1, Y2);
        std::swap(Z1, Z2);
        std::swap(Color1, Color2);
    }

    const int TotalHeight = Y2 - Y0;
    for (int Y = Y0; Y <= Y2; Y++)
    {
        const bool SecondHalf = Y > Y1 || Y1 == Y0;
        const int SegmentHeight = SecondHalf ? Y2 - Y1 : Y1 - Y0;
        const float Alpha = static_cast<float>(Y - Y0) / static_cast<float>(TotalHeight);
        const float Beta = static_cast<float>(Y - (SecondHalf ? Y1 : Y0)) / static_cast<float>(SegmentHeight);

        int Ax = X0 + static_cast<int>(static_cast<float>(X2 - X0) * Alpha);
        int Bx = SecondHalf ? X1 + static_cast<int>(static_cast<float>(X2 - X1) * Beta) : X0 + static_cast<int>(static_cast<float>(X1 - X0) * Beta);

        float Az = Z0 + (Z2 - Z0) * Alpha;
        float Bz = SecondHalf ? Z1 + (Z2 - Z1) * Beta : Z0 + (Z1 - Z0) * Beta;

        auto InterpolateColor = [](const uint32_t Color0, const uint32_t Color1, const float Alpha)
        {
            uint8_t R1 = (Color0 >> 16) & 0xFF, R2 = (Color1 >> 16) & 0xFF;
            uint8_t G1 = (Color0 >> 8) & 0xFF, G2 = (Color1 >> 8) & 0xFF;
            uint8_t B1 = Color0 & 0xFF, B2 = Color1 & 0xFF;

            const uint8_t R = R1 + static_cast<uint8_t>(static_cast<float>(R2 - R1) * Alpha);
            const uint8_t G = G1 + static_cast<uint8_t>(static_cast<float>(G2 - G1) * Alpha);
            const uint8_t B = B1 + static_cast<uint8_t>(static_cast<float>(B2 - B1) * Alpha);

            return (R << 16) | (G << 8) | B;
        };

        uint32_t ColorA = InterpolateColor(Color0, Color2, Alpha);
        uint32_t ColorB;
        if (SecondHalf) ColorB = InterpolateColor(Color1, Color2, Beta); // NOLINT(readability-suspicious-call-argument)
        else ColorB = InterpolateColor(Color0, Color1, Beta);

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

void Renderer::DrawTexture(const Texture* Texture)
{
    if (Texture == nullptr)
    {
        return;
    }

    auto Height = Texture->GetHeight();
    auto Width = Texture->GetWidth();

    // 居中显示
    const int X = (ScreenWidth - Width) / 2;
    const int Y = (ScreenHeight - Height) / 2;

    const int XStart = std::max(0, X);
    const int YStart = std::max(0, Y);
    const int XEnd = std::min(ScreenWidth, X + Width);
    const int YEnd = std::min(ScreenHeight, Y + Height);

    const float InvWidth = 1.0f / static_cast<float>(Width);
    const float InvHeight = 1.0f / static_cast<float>(Height);

    const int NumThreads = static_cast<int>(std::thread::hardware_concurrency());
    std::vector<std::thread> Threads(NumThreads);

    auto RenderSlice = [&](const int yStart, const int yEnd)
    {
        for (int y = yStart; y < yEnd; y++)
        {
            for (int x = XStart; x < XEnd; x++)
            {
                float u = static_cast<float>(x - X) * InvWidth;
                float v = static_cast<float>(y - Y) * InvHeight;
                Eigen::Vector3f color = Texture->Sample(Eigen::Vector2f(u, v));
                FrameBuffer[y * ScreenWidth + x] = ColorToUint32(color);
            }
        }
    };

    int SliceHeight = (YEnd - YStart) / NumThreads;
    for (int i = 0; i < NumThreads; ++i)
    {
        int yStart = YStart + i * SliceHeight;
        int yEnd = (i == NumThreads - 1) ? YEnd : yStart + SliceHeight;
        Threads[i] = std::thread(RenderSlice, yStart, yEnd);
    }

    for (auto& thread : Threads)
    {
        thread.join();
    }
}

void Renderer::ProcessTriangle(const Vertex& V1, const Vertex& V2, const Vertex& V3, const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix, const StandardVertexShader& VS, const FragmentShader& FS, const Material* Material)
{
    VertexShaderInput VSI1 = {V1.Position, V1.UV};
    VertexShaderInput VSI2 = {V2.Position, V2.UV};
    VertexShaderInput VSI3 = {V3.Position, V3.UV};

    VertexShaderOutput VSO1 = VS(VSI1, ModelMatrix, MVPMatrix);
    VertexShaderOutput VSO2 = VS(VSI2, ModelMatrix, MVPMatrix);
    VertexShaderOutput VSO3 = VS(VSI3, ModelMatrix, MVPMatrix);

    RasterizeTriangle(VSO1, VSO2, VSO3, FS, Material);
}

void Renderer::ProcessTriangle(const Vertex& V1, const Vertex& V2, const Vertex& V3, const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix, const Eigen::Matrix3f& NormalMatrix, const NormalVertexShader& VS, const SimpleLitFragmentShader& FS, const Material* Material, const DirectionalLight* DirectionalLightPtr)
{
    VertexShaderInput VSI1 = {V1.Position, V1.UV, V1.Normal};
    VertexShaderInput VSI2 = {V2.Position, V2.UV, V2.Normal};
    VertexShaderInput VSI3 = {V3.Position, V3.UV, V3.Normal};

    VertexShaderOutput VSO1 = VS(VSI1, ModelMatrix, MVPMatrix, NormalMatrix);
    VertexShaderOutput VSO2 = VS(VSI2, ModelMatrix, MVPMatrix, NormalMatrix);
    VertexShaderOutput VSO3 = VS(VSI3, ModelMatrix, MVPMatrix, NormalMatrix);

    RasterizeTriangle(VSO1, VSO2, VSO3, FS, Material, DirectionalLightPtr);
}

void Renderer::RasterizeTriangle(const VertexShaderOutput& V1, const VertexShaderOutput& V2, const VertexShaderOutput& V3, const FragmentShader& FS, const Material* Material)
{
    // 透视除法
    auto ProjectToScreen = [this](const Eigen::Vector4f& v) -> Eigen::Vector3f
    {
        Eigen::Vector3f NDC = v.head<3>() / v.w();
        return {
            (NDC.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth),
            (1.0f - NDC.y()) * 0.5f * static_cast<float>(ScreenHeight),
            NDC.z() // 保留深度信息
        };
    };

    Eigen::Vector3f P1 = ProjectToScreen(V1.Position);
    Eigen::Vector3f P2 = ProjectToScreen(V2.Position);
    Eigen::Vector3f P3 = ProjectToScreen(V3.Position);

    Eigen::Vector2i MinPoint(std::min({P1.x(), P2.x(), P3.x()}), std::min({P1.y(), P2.y(), P3.y()}));
    Eigen::Vector2i MaxPoint(std::max({P1.x(), P2.x(), P3.x()}), std::max({P1.y(), P2.y(), P3.y()}));

    const int NumThreads = static_cast<int>(std::thread::hardware_concurrency());
    std::vector<std::thread> Threads(NumThreads);

    int YStart = MinPoint.y();
    int YEnd = MaxPoint.y();
    int XStart = MinPoint.x();
    int XEnd = MaxPoint.x();

    auto DrawSlice = [&](const int yStart, const int yEnd)
    {
        for (int Y = yStart; Y <= yEnd; Y++)
        {
            for (int X = XStart; X <= XEnd; X++)
            {
                Eigen::Vector3f Barycentric = ComputeBarycentric(X, Y, P1.x(), P1.y(), P2.x(), P2.y(), P3.x(), P3.y());

                if (Barycentric.x() < 0 || Barycentric.y() < 0 || Barycentric.z() < 0) continue;

                float Depth = Barycentric.x() * P1.z() + Barycentric.y() * P2.z() + Barycentric.z() * P3.z();

                if (UseEarlyDepthTest)
                {
                    if (!EarlyDepthTest(X, Y, Depth)) continue;
                }

                FragmentShaderInput FSI;
                FSI.UV = Barycentric.x() * V1.UV + Barycentric.y() * V2.UV + Barycentric.z() * V3.UV;
                FSI.WorldPosition = Barycentric.x() * V1.WorldPosition + Barycentric.y() * V2.WorldPosition + Barycentric.z() * V3.WorldPosition;
                Eigen::Vector4f Color = FS(FSI, Material);

                DrawPixel(X, Y, Depth, ColorToUint32(Color));
            }
        }
    };

    int SliceHeight = (YEnd - YStart) / NumThreads;
    for (int i = 0; i < NumThreads; ++i)
    {
        int yStart = YStart + i * SliceHeight;
        int yEnd = (i == NumThreads - 1) ? YEnd : yStart + SliceHeight;
        Threads[i] = std::thread(DrawSlice, yStart, yEnd);
    }

    for (auto& Thread : Threads)
    {
        Thread.join();
    }
}

void Renderer::RasterizeTriangle(const VertexShaderOutput& V1, const VertexShaderOutput& V2, const VertexShaderOutput& V3, const SimpleLitFragmentShader& FS, const Material* Material, const DirectionalLight* DirectionalLightPtr)
{
    auto ProjectToScreen = [this](const Eigen::Vector4f& v) -> Eigen::Vector3f
    {
        Eigen::Vector3f NDC = v.head<3>() / v.w();
        return {
            (NDC.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth),
            (1.0f - NDC.y()) * 0.5f * static_cast<float>(ScreenHeight),
            NDC.z()
        };
    };

    Eigen::Vector3f P1 = ProjectToScreen(V1.Position);
    Eigen::Vector3f P2 = ProjectToScreen(V2.Position);
    Eigen::Vector3f P3 = ProjectToScreen(V3.Position);

    Eigen::Vector2i MinPoint(std::min({P1.x(), P2.x(), P3.x()}), std::min({P1.y(), P2.y(), P3.y()}));
    Eigen::Vector2i MaxPoint(std::max({P1.x(), P2.x(), P3.x()}), std::max({P1.y(), P2.y(), P3.y()}));

    const int NumThreads = static_cast<int>(std::thread::hardware_concurrency());
    std::vector<std::thread> Threads(NumThreads);

    int YStart = MinPoint.y();
    int YEnd = MaxPoint.y();
    int XStart = MinPoint.x();
    int XEnd = MaxPoint.x();

    auto DrawSlice = [&](const int yStart, const int yEnd)
    {
        for (int Y = yStart; Y <= yEnd; Y++)
        {
            for (int X = XStart; X <= XEnd; X++)
            {
                Eigen::Vector3f Barycentric = ComputeBarycentric(X, Y, P1.x(), P1.y(), P2.x(), P2.y(), P3.x(), P3.y());

                if (Barycentric.x() < 0 || Barycentric.y() < 0 || Barycentric.z() < 0) continue;

                float Depth = Barycentric.x() * P1.z() + Barycentric.y() * P2.z() + Barycentric.z() * P3.z();

                if (UseEarlyDepthTest)
                {
                    if (!EarlyDepthTest(X, Y, Depth)) continue;
                }

                FragmentShaderInput FSI;
                FSI.UV = Barycentric.x() * V1.UV + Barycentric.y() * V2.UV + Barycentric.z() * V3.UV;
                FSI.WorldPosition = Barycentric.x() * V1.WorldPosition + Barycentric.y() * V2.WorldPosition + Barycentric.z() * V3.WorldPosition;
                FSI.WorldNormal = Barycentric.x() * V1.WorldNormal + Barycentric.y() * V2.WorldNormal + Barycentric.z() * V3.WorldNormal;
                Eigen::Vector4f Color = FS(FSI, Material, DirectionalLightPtr);
                DrawPixel(X, Y, Depth, ColorToUint32(Color));
            }
        }
    };

    int SliceHeight = (YEnd - YStart) / NumThreads;
    for (int i = 0; i < NumThreads; ++i)
    {
        int yStart = YStart + i * SliceHeight;
        int yEnd = (i == NumThreads - 1) ? YEnd : yStart + SliceHeight;
        Threads[i] = std::thread(DrawSlice, yStart, yEnd);
    }

    for (auto& Thread : Threads)
    {
        Thread.join();
    }
}

void Renderer::RasterizeTriangle(const VertexShaderOutput& V1, const VertexShaderOutput& V2, const VertexShaderOutput& V3, const ShadowMapFragmentShader& FS, const Material* Material, const DirectionalLight* DirectionalLightPtr, const DepthTexture* ShadowMap, const Eigen::Matrix4f& LightSpaceMatrix)
{
    auto ProjectToScreen = [this](const Eigen::Vector4f& v) -> Eigen::Vector3f
    {
        Eigen::Vector3f NDC = v.head<3>() / v.w();
        return {
            (NDC.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth),
            (1.0f - NDC.y()) * 0.5f * static_cast<float>(ScreenHeight),
            NDC.z()
        };
    };

    Eigen::Vector3f P1 = ProjectToScreen(V1.Position);
    Eigen::Vector3f P2 = ProjectToScreen(V2.Position);
    Eigen::Vector3f P3 = ProjectToScreen(V3.Position);

    Eigen::Vector2i MinPoint(std::min({P1.x(), P2.x(), P3.x()}), std::min({P1.y(), P2.y(), P3.y()}));
    Eigen::Vector2i MaxPoint(std::max({P1.x(), P2.x(), P3.x()}), std::max({P1.y(), P2.y(), P3.y()}));

    const int NumThreads = static_cast<int>(std::thread::hardware_concurrency());
    std::vector<std::thread> Threads(NumThreads);

    int YStart = MinPoint.y();
    int YEnd = MaxPoint.y();
    int XStart = MinPoint.x();
    int XEnd = MaxPoint.x();

    auto DrawSlice = [&](const int yStart, const int yEnd)
    {
        for (int Y = yStart; Y <= yEnd; Y++)
        {
            for (int X = XStart; X <= XEnd; X++)
            {
                Eigen::Vector3f Barycentric = ComputeBarycentric(X, Y, P1.x(), P1.y(), P2.x(), P2.y(), P3.x(), P3.y());

                if (Barycentric.x() < 0 || Barycentric.y() < 0 || Barycentric.z() < 0) continue;

                float Depth = Barycentric.x() * P1.z() + Barycentric.y() * P2.z() + Barycentric.z() * P3.z();

                if (UseEarlyDepthTest)
                {
                    if (!EarlyDepthTest(X, Y, Depth)) continue;
                }

                FragmentShaderInput FSI;
                FSI.UV = Barycentric.x() * V1.UV + Barycentric.y() * V2.UV + Barycentric.z() * V3.UV;
                FSI.WorldPosition = Barycentric.x() * V1.WorldPosition + Barycentric.y() * V2.WorldPosition + Barycentric.z() * V3.WorldPosition;
                FSI.WorldNormal = Barycentric.x() * V1.WorldNormal + Barycentric.y() * V2.WorldNormal + Barycentric.z() * V3.WorldNormal;
                Eigen::Vector4f Color = FS(FSI, Material, DirectionalLightPtr, ShadowMap, LightSpaceMatrix);
                DrawPixel(X, Y, Depth, ColorToUint32(Color));
            }
        }
    };

    int SliceHeight = (YEnd - YStart) / NumThreads;
    for (int i = 0; i < NumThreads; ++i)
    {
        int yStart = YStart + i * SliceHeight;
        int yEnd = (i == NumThreads - 1) ? YEnd : yStart + SliceHeight;
        Threads[i] = std::thread(DrawSlice, yStart, yEnd);
    }

    for (auto& Thread : Threads)
    {
        Thread.join();
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

        const Mesh* MeshPtr = MeshObjectPtr->GetMesh();
        if (!MeshPtr) continue;

        for (size_t i = 0; i < MeshPtr->Indices.size(); i += 3)
        {
            const Vertex& V0 = MeshPtr->Vertices[MeshPtr->Indices[i]];
            const Vertex& V1 = MeshPtr->Vertices[MeshPtr->Indices[i + 1]];
            const Vertex& V2 = MeshPtr->Vertices[MeshPtr->Indices[i + 2]];

            Eigen::Vector4f Pos0 = MVPMatrix * V0.Position.homogeneous();
            Eigen::Vector4f Pos1 = MVPMatrix * V1.Position.homogeneous();
            Eigen::Vector4f Pos2 = MVPMatrix * V2.Position.homogeneous();

            float W0 = Pos0.w();
            float W1 = Pos1.w();
            float W2 = Pos2.w();

            Pos0 /= W0;
            Pos1 /= W1;
            Pos2 /= W2;

            int X0 = static_cast<int>((Pos0.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth));
            int Y0 = static_cast<int>((1.0f - Pos0.y()) * 0.5f * static_cast<float>(ScreenHeight));
            int X1 = static_cast<int>((Pos1.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth));
            int Y1 = static_cast<int>((1.0f - Pos1.y()) * 0.5f * static_cast<float>(ScreenHeight));
            int X2 = static_cast<int>((Pos2.x() + 1.0f) * 0.5f * static_cast<float>(ScreenWidth));
            int Y2 = static_cast<int>((1.0f - Pos2.y()) * 0.5f * static_cast<float>(ScreenHeight));

            FillTriangle(X0, Y0, Pos0.z(), 0x0000FF,
                         X1, Y1, Pos1.z(), 0x00FF00,
                         X2, Y2, Pos2.z(), 0x00FF00);
        }
    }
}

void Renderer::RenderScene(const Scene* Scene, const Camera* Camera, const RenderPipeline* Pipeline)
{
    if (Scene == nullptr || Camera == nullptr || Pipeline == nullptr)
    {
        return;
    }

    auto Objects = Scene->GetObjects();
    for (auto& ObjectUPtr : Objects)
    {
        Object* ObjectPtr = ObjectUPtr.get();
        if (ObjectPtr == nullptr)
        {
            continue;
        }
        MeshObject* MeshObjectPtr = dynamic_cast<MeshObject*>(ObjectPtr);
        if (MeshObjectPtr != nullptr)
        {
            RenderMeshObject(MeshObjectPtr, Camera, Pipeline);
        }
    }
}

void Renderer::RenderScene(const Scene* Scene, const Camera* Camera, const NormalRenderPipeline* Pipeline)
{
    if (Scene == nullptr || Camera == nullptr || Pipeline == nullptr)
    {
        return;
    }

    DirectionalLight* DirectionalLightPtr = Scene->GetDirectionalLight().get();

    BoundingBox SceneBoundingBox = Scene->CalculateSceneBounds();

    float depthBias = 0.005f;

    std::shared_ptr<DepthTexture> DepthTexturePtr = std::make_shared<DepthTexture>(512, 512);
    RenderShadowMap(Scene, DepthTexturePtr.get(), depthBias, DirectionalLightPtr, SceneBoundingBox);

    auto Objects = Scene->GetObjects();
    for (auto& ObjectUPtr : Objects)
    {
        Object* ObjectPtr = ObjectUPtr.get();
        if (ObjectPtr == nullptr)
        {
            continue;
        }
        bool bIsSceneHasLight = Scene->HasLight();
        MeshObject* MeshObjectPtr = dynamic_cast<MeshObject*>(ObjectPtr);
        if (MeshObjectPtr == nullptr)
        {
            return;
        }

        if (bIsSceneHasLight)
        {
            RenderMeshObject(MeshObjectPtr, Camera, Pipeline, DirectionalLightPtr, DepthTexturePtr.get(), DirectionalLightPtr->GetLightSpaceMatrix());
        }
    }

    Eigen::Matrix4f viewMatrix = Camera->GetViewMatrix();
    Eigen::Matrix4f projectionMatrix = Camera->GetProjectionMatrix();
    Eigen::Matrix4f viewProjectionMatrix = projectionMatrix * viewMatrix;
    DrawBoundingBox(SceneBoundingBox, viewProjectionMatrix, 0xFF0000);
}

void Renderer::RenderMeshObject(const MeshObject* MeshObject, const Camera* Camera, const RenderPipeline* Pipeline)
{
    if (MeshObject == nullptr || Camera == nullptr || Pipeline == nullptr)
    {
        return;
    }
    const Mesh* MeshPtr = MeshObject->GetMesh();
    const Material* MaterialPtr = MeshObject->GetMaterial();

    auto ModelMatrix = MeshObject->GetModelMatrix();
    auto ViewMatrix = Camera->GetViewMatrix();
    auto ProjectionMatrix = Camera->GetProjectionMatrix();
    auto MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

    for (size_t i = 0; i < MeshPtr->Indices.size(); i += 3)
    {
        const Vertex& V1 = MeshPtr->Vertices[MeshPtr->Indices[i]];
        const Vertex& V2 = MeshPtr->Vertices[MeshPtr->Indices[i + 1]];
        const Vertex& V3 = MeshPtr->Vertices[MeshPtr->Indices[i + 2]];

        ProcessTriangle(V1, V2, V3, ModelMatrix, MVPMatrix, Pipeline->VS, Pipeline->FS, MaterialPtr);
    }
}

void Renderer::RenderMeshObject(const MeshObject* MeshObject, const Camera* Camera, const NormalRenderPipeline* Pipeline, const DirectionalLight* DirectionalLightPtr)
{
    if (MeshObject == nullptr || Camera == nullptr || Pipeline == nullptr)
    {
        return;
    }

    const Mesh* MeshPtr = MeshObject->GetMesh();
    const Material* MaterialPtr = MeshObject->GetMaterial();

    auto ModelMatrix = MeshObject->GetModelMatrix();
    auto ViewMatrix = Camera->GetViewMatrix();
    auto ProjectionMatrix = Camera->GetProjectionMatrix();
    auto MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
    auto NormalMatrix = MeshObject->GetNormalMatrix();

    for (size_t i = 0; i < MeshPtr->Indices.size(); i += 3)
    {
        const Vertex& V1 = MeshPtr->Vertices[MeshPtr->Indices[i]];
        const Vertex& V2 = MeshPtr->Vertices[MeshPtr->Indices[i + 1]];
        const Vertex& V3 = MeshPtr->Vertices[MeshPtr->Indices[i + 2]];

        ProcessTriangle(V1, V2, V3, ModelMatrix, MVPMatrix, NormalMatrix, Pipeline->VS, Pipeline->FS, MaterialPtr, DirectionalLightPtr);
    }
}

void Renderer::RenderMeshObject(const MeshObject* MeshObject, const Camera* Camera, const NormalRenderPipeline* Pipeline, const DirectionalLight* DirectionalLightPtr, const DepthTexture* ShadowMap, const Eigen::Matrix4f& LightSpaceMatrix)
{
    if (MeshObject == nullptr || Camera == nullptr || Pipeline == nullptr)
    {
        return;
    }

    const Mesh* MeshPtr = MeshObject->GetMesh();
    const Material* MaterialPtr = MeshObject->GetMaterial();

    auto ModelMatrix = MeshObject->GetModelMatrix();
    auto ViewMatrix = Camera->GetViewMatrix();
    auto ProjectionMatrix = Camera->GetProjectionMatrix();
    auto MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
    auto NormalMatrix = MeshObject->GetNormalMatrix();

    for (size_t i = 0; i < MeshPtr->Indices.size(); i += 3)
    {
        const Vertex& V1 = MeshPtr->Vertices[MeshPtr->Indices[i]];
        const Vertex& V2 = MeshPtr->Vertices[MeshPtr->Indices[i + 1]];
        const Vertex& V3 = MeshPtr->Vertices[MeshPtr->Indices[i + 2]];

        VertexShaderInput VSI1 = {V1.Position, V1.UV, V1.Normal};
        VertexShaderInput VSI2 = {V2.Position, V2.UV, V2.Normal};
        VertexShaderInput VSI3 = {V3.Position, V3.UV, V3.Normal};

        auto VS = Pipeline->VS;

        VertexShaderOutput VSO1 = VS(VSI1, ModelMatrix, MVPMatrix, NormalMatrix);
        VertexShaderOutput VSO2 = VS(VSI2, ModelMatrix, MVPMatrix, NormalMatrix);
        VertexShaderOutput VSO3 = VS(VSI3, ModelMatrix, MVPMatrix, NormalMatrix);

        auto FS = DefaultShadowMapFragmentShader;
        RasterizeTriangle(VSO1, VSO2, VSO3, FS, MaterialPtr, DirectionalLightPtr, ShadowMap, LightSpaceMatrix);
    }
}

bool Renderer::EarlyDepthTest(int X, int Y, float Depth)
{
    if (X < 0 || X >= ScreenWidth || Y < 0 || Y >= ScreenHeight) return false;

    int index = Y * ScreenWidth + X;
    if (Depth >= DepthBuffer[index]) return false;

    DepthBuffer[index] = Depth;
    return true;
}

void Renderer::SetUseEarlyDepthTest(bool Enable)
{
    UseEarlyDepthTest = Enable;
}

bool isSaved = false;

void Renderer::RenderShadowMap(const Scene* Scene, DepthTexture* DepthTexture, float DepthBias, DirectionalLight* DirectionalLightPtr, const BoundingBox& SceneBoundingBox)
{
    Vec3f LightDirection = DirectionalLightPtr->Direction.normalized();
    Vec3f LightUp = Vec3f(0, 1, 0);
    if (std::abs(LightDirection.dot(LightUp)) > 0.99f)
    {
        LightUp = Vec3f(1, 0, 0);
    }
    Vec3f LightRight = LightDirection.cross(LightUp).normalized();
    LightUp = LightRight.cross(LightDirection).normalized();

    Vec3f SceneCenter = (SceneBoundingBox.Max + SceneBoundingBox.Min) * 0.5f;
    Eigen::Matrix4f LightViewMatrix;
    LightViewMatrix <<
        LightRight.x(), LightUp.x(), LightDirection.x(), 0,
        LightRight.y(), LightUp.y(), LightDirection.y(), 0,
        LightRight.z(), LightUp.z(), LightDirection.z(), 0,
        -LightRight.dot(SceneCenter), -LightUp.dot(SceneCenter), -LightDirection.dot(SceneCenter), 1;

    // 需要计算光源空间中的包围盒
    BoundingBox LightSpaceBoundingBox;
    std::vector Corners =
    {
        SceneBoundingBox.Min,
        SceneBoundingBox.Max,
        Vec3f(SceneBoundingBox.Min.x(), SceneBoundingBox.Min.y(), SceneBoundingBox.Max.z()),
        Vec3f(SceneBoundingBox.Min.x(), SceneBoundingBox.Max.y(), SceneBoundingBox.Min.z()),
        Vec3f(SceneBoundingBox.Min.x(), SceneBoundingBox.Max.y(), SceneBoundingBox.Max.z()),
        Vec3f(SceneBoundingBox.Max.x(), SceneBoundingBox.Min.y(), SceneBoundingBox.Max.z()),
        Vec3f(SceneBoundingBox.Max.x(), SceneBoundingBox.Max.y(), SceneBoundingBox.Min.z()),
        Vec3f(SceneBoundingBox.Max.x(), SceneBoundingBox.Min.y(), SceneBoundingBox.Min.z())
    };

    for (const auto& corner : Corners)
    {
        Vec4f lightSpaceCorner = LightViewMatrix * Vec4f(corner.x(), corner.y(), corner.z(), 1.0f);
        LightSpaceBoundingBox.Expand(Vec3f(lightSpaceCorner.x(), lightSpaceCorner.y(), lightSpaceCorner.z()));
    }

    float Left = LightSpaceBoundingBox.Min.x();
    float Right = LightSpaceBoundingBox.Max.x();
    float Bottom = LightSpaceBoundingBox.Min.y();
    float Top = LightSpaceBoundingBox.Max.y();
    float Near = LightSpaceBoundingBox.Min.z();
    float Far = LightSpaceBoundingBox.Max.z();

    float Padding = (Right - Left) * 0.1f;
    Left -= Padding;
    Right += Padding;
    Bottom -= Padding;
    Top += Padding;
    Near -= Padding;
    Far += Padding;

    Eigen::Matrix4f LightProjectionMatrix = Ortho(Left, Right, Bottom, Top, Near, Far);
    Eigen::Matrix4f LightSpaceMatrix = LightProjectionMatrix * LightViewMatrix;
    DirectionalLightPtr->SetLightSpaceMatrix(LightSpaceMatrix);

    for (const auto& Object : Scene->GetObjects())
    {
        const MeshObject* MeshObjectPtr = dynamic_cast<const MeshObject*>(Object.get());
        if (!MeshObjectPtr) continue;

        if (!MeshObjectPtr->GetCastShadow()) continue;

        Eigen::Matrix4f ModelMatrix = MeshObjectPtr->GetModelMatrix();
        Eigen::Matrix4f LightSpaceMVP = LightSpaceMatrix * ModelMatrix;

        RenderObjectDepth(MeshObjectPtr, DepthTexture, DepthBias, LightSpaceMVP);
    }

    if (!isSaved)
    {
        DepthTexture::SaveDepthTextureToBMP(DepthTexture, "ShadowMap.bmp");
    }
}

void Renderer::RenderObjectDepth(const MeshObject* MeshObject, DepthTexture* DepthTexture, float DepthBias, const Eigen::Matrix4f& LightSpaceMVP)
{
    const Mesh* MeshPtr = MeshObject->GetMesh();
    if (!MeshPtr) return;

    for (size_t i = 0; i < MeshPtr->Indices.size(); i += 3)
    {
        const Vertex& V1 = MeshPtr->Vertices[MeshPtr->Indices[i]];
        const Vertex& V2 = MeshPtr->Vertices[MeshPtr->Indices[i + 1]];
        const Vertex& V3 = MeshPtr->Vertices[MeshPtr->Indices[i + 2]];

        Eigen::Vector4f Pos1 = LightSpaceMVP * V1.Position.homogeneous();
        Eigen::Vector4f Pos2 = LightSpaceMVP * V2.Position.homogeneous();
        Eigen::Vector4f Pos3 = LightSpaceMVP * V3.Position.homogeneous();

        // 透视除法
        Pos1 /= Pos1.w();
        Pos2 /= Pos2.w();
        Pos3 /= Pos3.w();

        // 将NDC坐标转换为屏幕空间坐标
        auto ToScreenSpace = [DepthTexture](const Eigen::Vector4f& pos) -> Eigen::Vector3f
        {
            return Eigen::Vector3f(
                (pos.x() * 0.5f + 0.5f) * static_cast<float>(DepthTexture->Width),
                (pos.y() * 0.5f + 0.5f) * static_cast<float>(DepthTexture->Height),
                pos.z()
                );
        };

        Eigen::Vector3f ScreenPos1 = ToScreenSpace(Pos1);
        Eigen::Vector3f ScreenPos2 = ToScreenSpace(Pos2);
        Eigen::Vector3f ScreenPos3 = ToScreenSpace(Pos3);

        // 对三角形进行光栅化
        RasterizeTriangleDepth(ScreenPos1, ScreenPos2, ScreenPos3, DepthTexture);
    }
}

void Renderer::RasterizeTriangleDepth(const Eigen::Vector3f& V1, const Eigen::Vector3f& V2, const Eigen::Vector3f& V3, DepthTexture* DepthTexturePtr)
{
    if (!DepthTexturePtr) return;

    // 获取深度纹理的尺寸
    int width = DepthTexturePtr->GetWidth();
    int height = DepthTexturePtr->GetHeight();

    // 计算三角形的包围盒
    int minX = std::max(0, static_cast<int>(std::min({V1.x(), V2.x(), V3.x()})));
    int maxX = std::min(width - 1, static_cast<int>(std::max({V1.x(), V2.x(), V3.x()})));
    int minY = std::max(0, static_cast<int>(std::min({V1.y(), V2.y(), V3.y()})));
    int maxY = std::min(height - 1, static_cast<int>(std::max({V1.y(), V2.y(), V3.y()})));

    // 遍历包围盒中的每个像素
    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            // 计算像素中心点与三角形的重心坐标
            Eigen::Vector3f Barycentric = ComputeBarycentric(x + 0.5f, y + 0.5f, V1.x(), V1.y(), V2.x(), V2.y(), V3.x(), V3.y());

            // 如果像素中心点在三角形内部
            if (Barycentric.x() >= 0 && Barycentric.y() >= 0 && Barycentric.z() >= 0)
            {
                // 计算像素深度
                float Depth = Barycentric.x() * V1.z() + Barycentric.y() * V2.z() + Barycentric.z() * V3.z();

                // 更新深度纹理
                DepthTexturePtr->SetDepth(x, y, Depth);
            }
        }
    }
}

Eigen::Matrix4f Renderer::Ortho(const float Left, const float Right, const float Bottom, const float Top, const float Near, const float Far)
{
    Eigen::Matrix4f Result = Eigen::Matrix4f::Identity();
    Result(0, 0) = 2.0f / (Right - Left);
    Result(1, 1) = 2.0f / (Top - Bottom);
    Result(2, 2) = -1.0f / (Far - Near);
    Result(0, 3) = -(Right + Left) / (Right - Left);
    Result(1, 3) = -(Top + Bottom) / (Top - Bottom);
    Result(2, 3) = -Near / (Far - Near);
    return Result;
}

Eigen::Matrix4f Renderer::LookAt(const Eigen::Vector3f& Eye, const Eigen::Vector3f& Center, const Eigen::Vector3f& Up)
{
    Eigen::Vector3f AxisZ = (Eye - Center).normalized();
    Eigen::Vector3f AxisX = Up.cross(AxisZ).normalized();
    Eigen::Vector3f AxisY = AxisZ.cross(AxisX);

    Eigen::Matrix4f Result = Eigen::Matrix4f::Identity();
    Result <<
        AxisX.x(), AxisX.y(), AxisX.z(), -AxisX.dot(Eye),
        AxisY.x(), AxisY.y(), AxisY.z(), -AxisY.dot(Eye),
        AxisZ.x(), AxisZ.y(), AxisZ.z(), -AxisZ.dot(Eye),
        0, 0, 0, 1;

    return Result;
}

void Renderer::DrawBoundingBox(const BoundingBox& bbox, const Eigen::Matrix4f& viewProjectionMatrix, uint32_t color)
{
    std::vector<Eigen::Vector4f> vertices = {
        {bbox.Min.x(), bbox.Min.y(), bbox.Min.z(), 1.0f},
        {bbox.Max.x(), bbox.Min.y(), bbox.Min.z(), 1.0f},
        {bbox.Max.x(), bbox.Max.y(), bbox.Min.z(), 1.0f},
        {bbox.Min.x(), bbox.Max.y(), bbox.Min.z(), 1.0f},
        {bbox.Min.x(), bbox.Min.y(), bbox.Max.z(), 1.0f},
        {bbox.Max.x(), bbox.Min.y(), bbox.Max.z(), 1.0f},
        {bbox.Max.x(), bbox.Max.y(), bbox.Max.z(), 1.0f},
        {bbox.Min.x(), bbox.Max.y(), bbox.Max.z(), 1.0f}
    };

    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    // 局部函数：转换到屏幕空间
    auto ToScreenSpace = [this](const Eigen::Vector4f& clipPos) -> Eigen::Vector3f
    {
        Eigen::Vector3f ndcPos = clipPos.head<3>() / clipPos.w();
        return {
            (ndcPos.x() * 0.5f + 0.5f) * ScreenWidth,
            (ndcPos.y() * -0.5f + 0.5f) * ScreenHeight,
            ndcPos.z()
        };
    };

    // 局部函数：带深度测试的线段绘制
    auto DrawLineWithDepthTest = [this](const Eigen::Vector3f& start, const Eigen::Vector3f& end, uint32_t lineColor)
    {
        int x0 = static_cast<int>(start.x()), y0 = static_cast<int>(start.y());
        int x1 = static_cast<int>(end.x()), y1 = static_cast<int>(end.y());
        float z0 = start.z(), z1 = end.z();

        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        float dz = z1 - z0;
        float step = std::max(std::abs(dx), std::abs(dy));
        dz /= step;

        while (true)
        {
            if (x0 >= 0 && x0 < ScreenWidth && y0 >= 0 && y0 < ScreenHeight)
            {
                int index = y0 * ScreenWidth + x0;
                if (z0 < DepthBuffer[index])
                {
                    DepthBuffer[index] = z0;
                    FrameBuffer[index] = lineColor;
                }
            }
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy)
            {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                err += dx;
                y0 += sy;
            }
            z0 += dz;
        }
    };

    // 变换顶点到裁剪空间
    for (auto& vertex : vertices)
    {
        vertex = viewProjectionMatrix * vertex;
    }

    // 绘制边
    for (const auto& edge : edges)
    {
        const Eigen::Vector4f& start = vertices[edge.first];
        const Eigen::Vector4f& end = vertices[edge.second];

        // 简单的裁剪：如果两个端点都在近平面后面，跳过这条边
        if (start.w() <= 0 && end.w() <= 0) continue;

        Eigen::Vector3f screenStart = ToScreenSpace(start);
        Eigen::Vector3f screenEnd = ToScreenSpace(end);

        // 边界检查
        if (screenStart.x() < 0 || screenStart.x() >= ScreenWidth ||
            screenStart.y() < 0 || screenStart.y() >= ScreenHeight ||
            screenEnd.x() < 0 || screenEnd.x() >= ScreenWidth ||
            screenEnd.y() < 0 || screenEnd.y() >= ScreenHeight)
        {
            continue; // 跳过屏幕外的线段
        }

        DrawLineWithDepthTest(screenStart, screenEnd, color);
    }
}

Eigen::Vector3f Renderer::ComputeBarycentric(const int X, const int Y, const float X0, const float Y0, const float X1, const float Y1, const float X2, const float Y2)
{
    float Det = (X1 - X0) * (Y2 - Y0) - (X2 - X0) * (Y1 - Y0);
    float L1 = ((X1 - static_cast<float>(X)) * (Y2 - static_cast<float>(Y)) - (X2 - static_cast<float>(X)) * (Y1 - static_cast<float>(Y))) / Det;
    float L2 = ((X2 - static_cast<float>(X)) * (Y0 - static_cast<float>(Y)) - (X0 - static_cast<float>(X)) * (Y2 - static_cast<float>(Y))) / Det;
    float L3 = 1.0f - L1 - L2;
    return {L1, L2, L3};
}

bool Renderer::IsInsideTriangle(const int X, const int Y, const float X0, const float Y0, const float X1, const float Y1, const float X2, const float Y2)
{
    Eigen::Vector3f Barycentric = ComputeBarycentric(X, Y, X0, Y0, X1, Y1, X2, Y2);
    return Barycentric.x() >= 0 && Barycentric.y() >= 0 && Barycentric.z() >= 0;
}

uint32_t Renderer::ColorToUint32(const Eigen::Vector4f& Color)
{
    uint8_t R = static_cast<uint8_t>(std::min(std::max(Color.x() * 255.0f, 0.0f), 255.0f));
    uint8_t G = static_cast<uint8_t>(std::min(std::max(Color.y() * 255.0f, 0.0f), 255.0f));
    uint8_t B = static_cast<uint8_t>(std::min(std::max(Color.z() * 255.0f, 0.0f), 255.0f));
    return (R << 16) | (G << 8) | B;
}

uint32_t Renderer::ColorToUint32(const Eigen::Vector3f& Color)
{
    uint8_t R = static_cast<uint8_t>(std::min(std::max(Color.x() * 255.0f, 0.0f), 255.0f));
    uint8_t G = static_cast<uint8_t>(std::min(std::max(Color.y() * 255.0f, 0.0f), 255.0f));
    uint8_t B = static_cast<uint8_t>(std::min(std::max(Color.z() * 255.0f, 0.0f), 255.0f));
    return (R << 16) | (G << 8) | B;
}