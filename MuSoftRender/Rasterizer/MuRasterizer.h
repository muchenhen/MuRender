#pragma once
#include "../Math/MuMath.h"
#include "../Device/MuDevice.h"
#include "../Function/MuStruct.h"

#include "../Obj/MuObjModel.h"

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

    bool DrawPoint(FrameBuffer* PointBitFrameBuffer, const MuPoint3I& Point, const MuRGB& Color);

    bool DrawPoint(FrameBuffer* PointBitFrameBuffer, const MuPoint4I& Point, const MuRGB& Color);

    /**
     * \brief 画线，使用Bresenham算法
     * \param PointBitFrameBuffer 
     * \param StartPoint 
     * \param EndPoint 
     * \param Color 
     * \return 绘制结果
     */
    bool DrawLine(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& StartPoint, const MuPoint2I& EndPoint, const MuRGB& Color);

    bool DrawLine(FrameBuffer* PointBitFrameBuffer, const MuPoint2F& StartPoint, const MuPoint2F& EndPoint, const MuRGB& Color);

    bool DrawLine(MuDevice* Device, const MuPoint3F& StartPoint, const MuPoint3F& EndPoint, const MuRGB& Color);
    
    bool DrawTriangle(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& Point1, const MuPoint2I& Point2, const MuPoint2I& Point3, const MuRGB& Color);

    bool DrawTriangle(FrameBuffer* PointBitFrameBuffer, const MuPoint2F& Point1, const MuPoint2F& Point2, const MuPoint2F& Point3, const MuRGB& Color);

    bool DrawTriangle(MuDevice* Device, const MuPoint3F& Point1, const MuPoint3F& Point2, const MuPoint3F& Point3, const MuRGB& Color);

    // 画出实心三角形
    bool DrawTriangleSolid(FrameBuffer* PointBitFrameBuffer, const MuPoint2F& Point1, const MuPoint2F& Point2, const MuPoint2F& Point3);
    
    bool DrawQuad(FrameBuffer* PointBitFrameBuffer, const MuPoint2I& Point1, const MuPoint2I& Point2, const MuPoint2I& Point3, const MuPoint2I& Point4, const MuRGB& Color);

    bool DrawObj(MuDevice* Device, MuObjModel* ObjModel, const MuRGB& Color);
    
    MuVector2I GetRandomPoint2I();
    
    void RandomDraw(FrameBuffer* PointBitFrameBuffer);

    void RandomDrawTriangle(FrameBuffer* PointBitFrameBuffer);

    void RandomDrawQuad(FrameBuffer* PointBitFrameBuffer);
};
