#include "MuRasterizer.h"
#include "../Function//MuLog.h"
#include"MuMath.h"

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
    // 确保点没有超出屏幕范围
    if (Point.x() >= 0 && Point.x() < WidthBound && Point.y() >= 0 && Point.y() < HeightBound)
    {
        PointBitFrameBuffer[WidthBound * Point.y() + Point.x()] = MuColor::MuRGBtoUInt(Color);
        return true;
    }
    return false;
}

bool MuRasterizer::DrawPoint(unsigned* PointBitFrameBuffer, const MuPoint3I& Point, const MuRGB& Color)
{
    const MuPoint2I Point2D = MuMath::Point3IToPoint2I(Point);
	return DrawPoint(PointBitFrameBuffer, Point2D, Color);
}

bool MuRasterizer::DrawPoint(unsigned* PointBitFrameBuffer, const MuPoint4I& Point, const MuRGB& Color)
{
    const MuPoint2I Point2D = MuMath::Point4IToPoint2I(Point);
    return DrawPoint(PointBitFrameBuffer, Point2D, Color);
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

bool MuRasterizer::DrawQuad(unsigned* PointBitFrameBuffer, const MuPoint2I& Point1, const MuPoint2I& Point2, const MuPoint2I& Point3, const MuPoint2I& Point4, const MuRGB& Color)
{
    MuPoint2I CenterPoint;
    // 求出四个点的中心
    CenterPoint = (Point1 + Point2 + Point3 + Point4) / 4;
    // 以中心点为原点，x轴正方向开始逆时针排序四个点
    vector<MuPoint2I> SortedPoint = { Point1, Point2, Point3, Point4 };
    sort(SortedPoint.begin(), SortedPoint.end(), [CenterPoint](const MuPoint2I& Point1, const MuPoint2I& Point2)
        {
            const MuPoint2I Vector1 = Point1 - CenterPoint;
            const MuPoint2I Vector2 = Point2 - CenterPoint;
            const float Angle1 = atan2(Vector1.y(), Vector1.x());
            const float Angle2 = atan2(Vector2.y(), Vector2.x());
            return Angle1 < Angle2;
        });
    // 按照顺序，12 23 34 41画线
    if (DrawLine(PointBitFrameBuffer, SortedPoint[0], SortedPoint[1], Color) &&
        DrawLine(PointBitFrameBuffer, SortedPoint[1], SortedPoint[2], Color) &&
        DrawLine(PointBitFrameBuffer, SortedPoint[2], SortedPoint[3], Color) &&
        DrawLine(PointBitFrameBuffer, SortedPoint[3], SortedPoint[0], Color))
    {
        return true;
    }

    return true;
}

bool MuRasterizer::DrawObj(unsigned* PointBitFrameBuffer, MuObjModel* ObjModel, const MuRGB& Color)
{
    const FMuObjFace Face = ObjModel->GetFace(0);
    const int ObjFaceVertexCount = Face.GetVertexCount();
    MuLog::LogInfo("DrawObj: Face Vertex Count = %d", ObjFaceVertexCount);
    // 画出obj模型
    for (int i = 0; i < ObjModel->GetFaceCount(); i++)
    {
        // 确认 Face 顶点数量
        if (ObjFaceVertexCount == 3)
        {
            const int VertexIndex1 = Face.GetVertex(0).VertexIndex;
            const int VertexIndex2 = Face.GetVertex(1).VertexIndex;
            const int VertexIndex3 = Face.GetVertex(2).VertexIndex;
            
            const MuPoint2I Point1 = ObjModel->GetVertexByIndex(VertexIndex1);
            const MuPoint2I Point2 = ObjModel->GetVertexByIndex(VertexIndex2);
            const MuPoint2I Point3 = ObjModel->GetVertexByIndex(VertexIndex3);
            // DrawTriangle(PointBitFrameBuffer, Point1, Point2, Point3, Color);
        }
        else if (ObjFaceVertexCount == 4)
        {
            const int VertexIndex1 = Face.GetVertex(0).VertexIndex;
            const int VertexIndex2 = Face.GetVertex(1).VertexIndex;
            const int VertexIndex3 = Face.GetVertex(2).VertexIndex;
            const int VertexIndex4 = Face.GetVertex(3).VertexIndex;
            MuPoint2I Point1 = MuMath::Point3IToPoint2I(ObjModel->GetVertexByIndex(VertexIndex1).cast<int>());
            MuPoint2I Point2 = MuMath::Point3IToPoint2I(ObjModel->GetVertexByIndex(VertexIndex2).cast<int>());
            MuPoint2I Point3 = MuMath::Point3IToPoint2I(ObjModel->GetVertexByIndex(VertexIndex3).cast<int>());
            MuPoint2I Point4 = MuMath::Point3IToPoint2I(ObjModel->GetVertexByIndex(VertexIndex4).cast<int>());
            // 打印四个点的坐标 x y
            MuLog::LogInfo("DrawObj: Point1 = %d %d", Point1.x(), Point1.y());
            MuLog::LogInfo("DrawObj: Point2 = %d %d", Point2.x(), Point2.y());
            MuLog::LogInfo("DrawObj: Point3 = %d %d", Point3.x(), Point3.y());
            MuLog::LogInfo("DrawObj: Point4 = %d %d", Point4.x(), Point4.y());
            DrawQuad(PointBitFrameBuffer, Point1, Point2, Point3, Point4, Color);
        }
    }
    return true;
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

void MuRasterizer::RandomDrawQuad(unsigned* PointBitFrameBuffer)
{
    // 在屏幕上随机取四个点，然后画四边形
    const MuPoint2I Point1 = GetRandomPoint2I();
    const MuPoint2I Point2 = GetRandomPoint2I();
    const MuPoint2I Point3 = GetRandomPoint2I();
    const MuPoint2I Point4 = GetRandomPoint2I();
    // print point
    MuLog::LogInfo("Point1: %d, %d", Point1.x(), Point1.y());
    MuLog::LogInfo("Point2: %d, %d", Point2.x(), Point2.y());
    MuLog::LogInfo("Point3: %d, %d", Point3.x(), Point3.y());
    MuLog::LogInfo("Point4: %d, %d", Point4.x(), Point4.y());
    DrawQuad(PointBitFrameBuffer, Point1, Point2, Point3, Point4, MuColor::GetRandomMuRGB());
}
