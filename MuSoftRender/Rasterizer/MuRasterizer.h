#pragma once
#include "../Math/MuMath.h"
#include "../Device/MuDevice.h"

class MuRasterizer
{
private:
    int WidthBound;
    int HeightBound;

public:

    MuRasterizer();

    void InitRasterizer(int Width, int Height);

    /**
     * \brief 在指定位置，用指定颜色，画点，PointBitFrameBuffer是一个一维数组，用来存储位图的颜色，它的大小是WidthBound * HeightBound
     *         一行一行的存储，每一行的颜色是从左到右
     * \param PointBitFrameBuffer 位图颜色缓存
     * \param Point 画点的位置
     * \param Color 点的颜色
     * \return 是否画成功了
     */
    bool DrawPoint(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& Point, const MuRGB& Color);

    bool DrawLine(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& StartPoint, const MuPoint2I& EndPoint, const MuRGB& Color);

    bool DrawTriangle(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& Point1, const MuPoint2I& Point2, const MuPoint2I& Point3, const MuRGB& Color);

    MuVector2I GetRandomPoint2I();
    
    void RandomDraw(FrameBuffer* PointBitFrameBuffer);

    void RandomDrawTriangle(FrameBuffer* PointBitFrameBuffer);
};