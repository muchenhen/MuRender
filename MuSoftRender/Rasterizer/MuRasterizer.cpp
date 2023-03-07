#include "MuRasterizer.h"
#include "../Function//MuLog.h"

MuRasterizer::MuRasterizer():
    WidthBound(0),
    HeightBound(0)
{
}

void MuRasterizer::InitRasterizer(int Width, int Height)
{
    WidthBound = Width;
    HeightBound = Height;
}

bool MuRasterizer::DrawPoint(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& Point, const MuRGB& Color)
{
    // 保定点没有超出屏幕范围
    if (Point.x() >= 0 && Point.x() < WidthBound && Point.y() >= 0 && Point.y() < HeightBound)
    {
        PointBitFrameBuffer[WidthBound * Point.y() + Point.x()] = MuColor::MuRGBtoUInt(Color);
        return true;
    }
    return false;
}

bool MuRasterizer::DrawLine(unsigned* PointBitFrameBuffer, const MuPoint2I& StartPoint, const MuPoint2I& EndPoint, const MuRGB& Color)
{
    // Bresenham算法
    int X0 = StartPoint(0);
    int Y0 = StartPoint(1);
    const int X1 = EndPoint(0);
    const int Y1 = EndPoint(1);
    const int dX = abs(X1 - X0);
    const int dY = abs(Y1 - Y0);
    const int sX = (X0 < X1) ? 1 : -1; // 增量方向
    const int sY = (Y0 < Y1) ? 1 : -1;
    int Err = dX - dY;
    while (true)
    {
        DrawPoint(PointBitFrameBuffer, MuPoint2I(X0, Y0), Color);
        if (X0 == X1 && Y0 == Y1)
        {
            break;
        }
        const int E2 = Err * 2;
        if (E2 > -dY)
        {
            Err = Err - dY;
            X0 = X0 + sX;
        }
        if (E2 < dX)
        {
            Err = Err + dX;
            Y0 = Y0 + sY;
        }
    }
    return true;
}

bool MuRasterizer::DrawTriangle(unsigned* PointBitFrameBuffer, const MuPoint2I& Point1, const MuPoint2I& Point2, const MuPoint2I& Point3, const MuRGB& Color)
{
    if (DrawLine(PointBitFrameBuffer, Point1, Point2, Color) &&
        DrawLine(PointBitFrameBuffer, Point2, Point3, Color) &&
        DrawLine(PointBitFrameBuffer, Point3, Point1, Color))
    {
        return true;
    }
    return false;
}

MuVector2I MuRasterizer::GetRandomPoint2I()
{
    // 在widthBound * heightBound范围内随机取一个点
    const int X = MuMath::RandomInt(0, WidthBound);
    const int Y = MuMath::RandomInt(0, HeightBound);
    return MuVector2I{X, Y};
}

void MuRasterizer::RandomDraw(unsigned* PointBitFrameBuffer)
{
    for (int i = 0; i < WidthBound * HeightBound; i++)
    {
        PointBitFrameBuffer[i] = MuColor::GetRandomColor();
    }
}

void MuRasterizer::RandomDrawTriangle(unsigned* PointBitFrameBuffer)
{
    // 在屏幕上随机取三个点，然后画三角形
    const MuPoint2I Point1 = GetRandomPoint2I();
    const MuPoint2I Point2 = GetRandomPoint2I();
    const MuPoint2I Point3 = GetRandomPoint2I();
    // print point
    MuLog::LogInfo("Point1: %d, %d", Point1.x(), Point1.y());
    MuLog::LogInfo("Point2: %d, %d", Point2.x(), Point2.y());
    MuLog::LogInfo("Point3: %d, %d", Point3.x(), Point3.y());
    DrawTriangle(PointBitFrameBuffer, Point1, Point2, Point3, MuColor::GetRandomMuRGB());
}
