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
    // LOG_DEBUG("DrawPixel: X = %d, Y = %d, Color = 0x%08X", X, Y, Color);
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

void Renderer::DrawTexture(Texture* Texture)
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

    const float InvWidth = 1.0f / Width;
    const float InvHeight = 1.0f / Height;

    #pragma omp parallel for
    for (int y = YStart; y < YEnd; ++y)
    {
        const float v = static_cast<float>(y - Y) * InvHeight;
        uint32_t* row = &FrameBuffer[y * ScreenWidth + XStart];
        
        for (int x = XStart; x < XEnd; ++x)
        {
            const float u = static_cast<float>(x - X) * InvWidth;
            const Eigen::Vector3f color = Texture->Sample(Eigen::Vector2f(u, v));

            const uint32_t r = static_cast<uint32_t>(color.x() * 255);
            const uint32_t g = static_cast<uint32_t>(color.y() * 255);
            const uint32_t b = static_cast<uint32_t>(color.z() * 255);

            *row++ = (255 << 24) | (r << 16) | (g << 8) | b;
        }
    }

    // for (int y = Y; y < ScreenHeight; ++y)
    // {
    //     for (int x = X; x < ScreenWidth; ++x)
    //     {
    //         float u = static_cast<float>(x) / ScreenWidth;
    //         float v = static_cast<float>(y) / ScreenHeight;
    //         Eigen::Vector3f color = Texture->Sample(Eigen::Vector2f(u, v));
    //
    //         uint8_t r = static_cast<uint8_t>(color.x() * 255);
    //         uint8_t g = static_cast<uint8_t>(color.y() * 255);
    //         uint8_t b = static_cast<uint8_t>(color.z() * 255);
    //
    //         FrameBuffer[y * ScreenWidth + x] = (255 << 24) | (r << 16) | (g << 8) | b;
    //     }
    // }
}

void Renderer::ProcessTriangle(const Vertex& V1, const Vertex& V2, const Vertex& V3, const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix, const VertexShader& VS, const FragmentShader& FS, const Material* Material)
{
    VertexShaderInput VSI1 = {V1.Position, V1.UV};
    VertexShaderInput VSI2 = {V2.Position, V2.UV};
    VertexShaderInput VSI3 = {V3.Position, V3.UV};

    VertexShaderOutput VSO1 = VS(VSI1, ModelMatrix, MVPMatrix);
    VertexShaderOutput VSO2 = VS(VSI2, ModelMatrix, MVPMatrix);
    VertexShaderOutput VSO3 = VS(VSI3, ModelMatrix, MVPMatrix);

    RasterizeTriangle(VSO1, VSO2, VSO3, FS, Material);
}

void Renderer::RasterizeTriangle(const VertexShaderOutput& V1, const VertexShaderOutput& V2, const VertexShaderOutput& V3, const FragmentShader& FS, const Material* Material)
{
    // 透视除法
    auto ProjectToScreen = [this](const Eigen::Vector4f& v)
    {
        Eigen::Vector3f NDC = v.head<3>() / v.w();
        return Eigen::Vector2i(
            static_cast<int>((NDC.x() + 1.0f) * 0.5f * ScreenWidth),
            static_cast<int>((1.0f - NDC.y()) * 0.5f * ScreenHeight)
            );
    };

    Eigen::Vector2i P1 = ProjectToScreen(V1.Position);
    Eigen::Vector2i P2 = ProjectToScreen(V2.Position);
    Eigen::Vector2i P3 = ProjectToScreen(V3.Position);

    Eigen::Vector2i MinPoint(std::min({P1.x(), P2.x(), P3.x()}), std::min({P1.y(), P2.y(), P3.y()}));
    Eigen::Vector2i MaxPoint(std::max({P1.x(), P2.x(), P3.x()}), std::max({P1.y(), P2.y(), P3.y()}));

    for (int Y = MinPoint.y(); Y <= MaxPoint.y(); Y++)
    {
        for (int X = MinPoint.x(); X <= MaxPoint.x(); X++)
        {
            Eigen::Vector3f Barycentric = ComputeBarycentric(X, Y, P1.x(), P1.y(), P2.x(), P2.y(), P3.x(), P3.y());
            if (Barycentric.x() < 0 || Barycentric.y() < 0 || Barycentric.z() < 0) continue;
            FragmentShaderInput FSI;
            FSI.UV = Barycentric.x() * V1.UV + Barycentric.y() * V2.UV + Barycentric.z() * V3.UV;
            FSI.WorldPosition = Barycentric.x() * V1.WorldPosition + Barycentric.y() * V2.WorldPosition + Barycentric.z() * V3.WorldPosition;
            Eigen::Vector4f Color = FS(FSI, Material);

            DrawPixel(X, Y, ColorToUint32(Color));
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

        const Mesh* MeshPtr = MeshObjectPtr->GetMesh();
        if (!MeshPtr) continue;

        const Material* MaterialPtr = MeshObjectPtr->GetMaterial();

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

void Renderer::RenderScene(Scene* Scene, Camera* Camera, RenderPipeline* Pipeline)
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

void Renderer::RenderMeshObject(MeshObject* MeshObject, Camera* Camera, RenderPipeline* Pipeline)
{
    if (MeshObject == nullptr || Camera == nullptr || Pipeline == nullptr)
    {
        return;
    }
    const Mesh* MeshPtr = MeshObject->GetMesh();
    const Material* MaterialPtr = MeshObject->GetMaterial();
    if (MaterialPtr == nullptr)
    {
        return;
    }

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

Eigen::Vector3f Renderer::ComputeBarycentric(int X, int Y, int X0, int Y0, int X1, int Y1, int X2, int Y2)
{
    float Det = (X1 - X0) * (Y2 - Y0) - (X2 - X0) * (Y1 - Y0);
    float L1 = ((X1 - X) * (Y2 - Y) - (X2 - X) * (Y1 - Y)) / Det;
    float L2 = ((X2 - X) * (Y0 - Y) - (X0 - X) * (Y2 - Y)) / Det;
    float L3 = 1.0f - L1 - L2;
    return Eigen::Vector3f(L1, L2, L3);
}

bool Renderer::IsInsideTriangle(int X, int Y, int X0, int Y0, int X1, int Y1, int X2, int Y2)
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