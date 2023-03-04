#include "Colors.h"
#include "Device.h"

void Device::DrawPoint(int x, int y, const unsigned int color, const float z)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        pointFrameBuffer[width * y + x] = color;
        zBuffer[width * y + x] = z;
    }
}

void Device::DrawLine(int x0, int y0, int x1, int y1, const MuVector& color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int ux = (dx > 0) ? 1 : -1;
    int uy = (dy > 0) ? 1 : -1;
    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    if (dx > dy)
    {
        int e = 2 * dy - dx;
        int y = y0;
        for (int x = x0; x != x1; x += ux)
        {
            DrawPoint(x, y, MathSet::color2UINT(color), 0);
            e += dy;
            if (e > 0)
            {
                y += uy;
                e -= dx;
            }
        }
    }
    else
    {
        int e = 2 * dx - dy;
        int x = x0;
        for (int y = y0; y != y1; y += uy)
        {
            DrawPoint(x, y, MathSet::color2UINT(color), 0);
            e += dx;
            if (e >= 0)
            {
                x += ux;
                e -= dy;
            }
        }
    }
}

void Device::DrawLine(const MuVector& p0, const MuVector& p1, const MuVector& color)
{
    int x0 = (int)floor(p0.X);
    int x1 = (int)floor(p1.X);
    int y0 = (int)floor(p0.Y);
    int y1 = (int)floor(p1.Y);
    DrawLine(x0, y0, x1, y1, color);
}

bool Device::TriangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, MuVector& Vw)
{
    Vw.X = MathSet::edgeFunc(v1.pos, v2.pos, p.pos) * v0.pos.W / Vw.W;
    Vw.Y = MathSet::edgeFunc(v2.pos, v0.pos, p.pos) * v1.pos.W / Vw.W;
    Vw.Z = MathSet::edgeFunc(v0.pos, v1.pos, p.pos) * v2.pos.W / Vw.W;
    return (Vw.X >= 0 && Vw.Y >= 0 && Vw.Z >= 0);
    return false;
}

bool Device::BackFaceCulling(const MuVector& p0, const MuVector& p1, const MuVector& p2)
{
    return ((p0 * MuVector(MultiplicationCross((p2 - p0), (p1 - p0)))) > 0);
}

void Device::FillTriangleVertexColor(Vertex& v0, Vertex& v1, Vertex& v2)
{
    float xmax = MathSet::max3(v0.pos.X, v1.pos.X, v2.pos.X);
    float ymax = MathSet::max3(v0.pos.Y, v1.pos.Y, v2.pos.Y);
    float xmin = MathSet::min3(v0.pos.X, v1.pos.X, v2.pos.X);
    float ymin = MathSet::min3(v0.pos.Y, v1.pos.Y, v2.pos.Y);

    int x0 = std::max<>(0, (int)std::floor(xmin));
    int y0 = std::max<>(0, (int)std::floor(ymin));
    int x1 = std::min<>(width - 1, (int)std::floor(xmax));
    int y1 = std::min<>(height - 1, (int)std::floor(ymax));

    MuVector Vweight = {0, 0, 0, MathSet::edgeFunc(v0.pos, v1.pos, v2.pos)};

    if (optimized)
    {
        float w0base = (v2.pos.X - v1.pos.X) * (y0 + 0.5f - v1.pos.Y) - (v2.pos.Y - v1.pos.Y) * (x0 + 0.5f - v1.pos.X);
        float w1base = (v0.pos.X - v2.pos.X) * (y0 + 0.5f - v2.pos.Y) - (v0.pos.Y - v2.pos.Y) * (x0 + 0.5f - v2.pos.X);
        float w2base = (v1.pos.X - v0.pos.X) * (y0 + 0.5f - v0.pos.Y) - (v1.pos.Y - v0.pos.Y) * (x0 + 0.5f - v0.pos.X);
        float w0stepX = -(v2.pos.Y - v1.pos.Y);
        float w0stepY = (v2.pos.X - v1.pos.X);
        float w1stepX = -(v0.pos.Y - v2.pos.Y);
        float w1stepY = (v0.pos.X - v2.pos.X);
        float w2stepX = -(v1.pos.Y - v0.pos.Y);
        float w2stepY = (v1.pos.X - v0.pos.X);
        for (int x = x0, sx = 0; x <= x1; x++, sx++)
        {
            for (int y = y0, sy = 0; y <= y1; y++, sy++)
            {
                Vertex samplePixel = {{x + 0.5f, y + 0.5f, 0.0f, 0.0f}, {}};

                float w0 = w0base + w0stepX * sx + w0stepY * sy;
                float w1 = w1base + w1stepX * sx + w1stepY * sy;
                float w2 = w2base + w2stepX * sx + w2stepY * sy;

                if (!(w0 >= 0 && w1 >= 0 && w2 >= 0))
                {
                    continue;
                }

                Vweight.X = w0 * v0.pos.W / Vweight.W;
                Vweight.Y = w1 * v1.pos.W / Vweight.W;
                Vweight.Z = w2 * v2.pos.W / Vweight.W;

                Interpolate(v0, v1, v2, samplePixel, Vweight);

                DrawPoint(x, y, MathSet::color2UINT(samplePixel.color), samplePixel.pos.Z);
            }
        }
    }
    else
    {
        for (int x = x0; x <= x1; x++)
        {
            for (int y = y0; y <= y1; y++)
            {
                Vertex samplePixel = {{x + 0.5f, y + 0.5f, 0.0f, 0.0f}, {}};
                if (!TriangleCheck(v0, v1, v2, samplePixel, Vweight))
                {
                    continue;
                }
                Interpolate(v0, v1, v2, samplePixel, Vweight);
                DrawPoint(x, y, MathSet::color2UINT(samplePixel.color), samplePixel.pos.Z);
            }
        }
    }
}

void Device::FillTriangleTexture(Vertex& v0, Vertex& v1, Vertex& v2)
{
    float xmax = MathSet::max3(v0.pos.X, v1.pos.X, v2.pos.X);
    float ymax = MathSet::max3(v0.pos.Y, v1.pos.Y, v2.pos.Y);
    float xmin = MathSet::min3(v0.pos.X, v1.pos.X, v2.pos.X);
    float ymin = MathSet::min3(v0.pos.Y, v1.pos.Y, v2.pos.Y);
    int x0 = std::max<>(0, (int)std::floor(xmin));
    int y0 = std::max<>(0, (int)std::floor(ymin));
    int x1 = std::min<>(width - 1, (int)std::floor(xmax));
    int y1 = std::min<>(height - 1, (int)std::floor(ymax));
    MuVector Vweight = {0, 0, 0, MathSet::edgeFunc(v0.pos, v1.pos, v2.pos)};
    float w0base = (v2.pos.X - v1.pos.X) * (y0 + 0.5f - v1.pos.Y) - (v2.pos.Y - v1.pos.Y) * (x0 + 0.5f - v1.pos.X);
    float w1base = (v0.pos.X - v2.pos.X) * (y0 + 0.5f - v2.pos.Y) - (v0.pos.Y - v2.pos.Y) * (x0 + 0.5f - v2.pos.X);
    float w2base = (v1.pos.X - v0.pos.X) * (y0 + 0.5f - v0.pos.Y) - (v1.pos.Y - v0.pos.Y) * (x0 + 0.5f - v0.pos.X);
    float w0_stepX = -(v2.pos.Y - v1.pos.Y);
    float w0_stepY = (v2.pos.X - v1.pos.X);
    float w1_stepX = -(v0.pos.Y - v2.pos.Y);
    float w1_stepY = (v0.pos.X - v2.pos.X);
    float w2_stepX = -(v1.pos.Y - v0.pos.Y);
    float w2_stepY = (v1.pos.X - v0.pos.X);
    for (int x = x0, sx = 0; x <= x1; x++, sx++)
        for (int y = y0, sy = 0; y <= y1; y++, sy++)
        {
            Vertex SamplePixel = {{x + 0.5f, y + 0.5f, 0.f, 0.f}, {}};
            float w0 = w0base + w0_stepX * sx + w0_stepY * sy;
            float w1 = w1base + w1_stepX * sx + w1_stepY * sy;
            float w2 = w2base + w2_stepX * sx + w2_stepY * sy;

            if (!(w0 >= 0 && w1 >= 0 && w2 >= 0)) continue;
            Vweight.X = w0 * v0.pos.W / Vweight.W;
            Vweight.Y = w1 * v1.pos.W / Vweight.W;
            Vweight.Z = w2 * v2.pos.W / Vweight.W;
            Interpolate(v0, v1, v2, SamplePixel, Vweight);
            DrawPoint(x, y, pTexture->SampleColor(SamplePixel.uv), SamplePixel.pos.Z);
        }
}

void Device::Rasterize(Vertex& v0, Vertex& v1, Vertex& v2)
{
    if (renderMode == RenderMode::RenderStateWireFrame)
    {
        DrawLine(v0.pos, v1.pos, ColorsL::White);
        DrawLine(v0.pos, v2.pos, ColorsL::White);
        DrawLine(v1.pos, v2.pos, ColorsL::White);
    }
    else if (renderMode == RenderMode::RenderStateColor)
    {
        FillTriangleVertexColor(v0, v1, v2);
    }
    else
    {
        FillTriangleTexture(v0, v1, v2);
    }
}
