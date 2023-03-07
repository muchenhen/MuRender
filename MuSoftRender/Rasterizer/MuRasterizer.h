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
     * \brief 在指定位置，用指定颜色，画点
     * \param PointBitFrameBuffer 位图颜色缓存
     * \param Point 画点的位置
     * \param Color 点的颜色
     * \return 是否画成功了
     */
    bool DrawPoint(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& Point, const MuRGB& Color);

    void RandomDraw(FrameBuffer* PointBitFrameBuffer);
};
