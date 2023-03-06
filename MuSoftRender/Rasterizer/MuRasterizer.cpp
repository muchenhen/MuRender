#include "MuRasterizer.h"

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
