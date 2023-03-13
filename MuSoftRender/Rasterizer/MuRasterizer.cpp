#include "MuRasterizer.h"
#include "../Function//MuLog.h"
#include "../Math/MuMath.h"

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

bool MuRasterizer::DrawLine(FrameBuffer* PointBitFrameBuffer, const MuPoint2F& StartPoint, const MuPoint2F& EndPoint, const MuRGB& Color)
{
    // 将FLoat点转换为Int点
    const MuPoint2I StartPointInt = MuMath::Point2FToPoint2I(StartPoint);
    const MuPoint2I EndPointInt = MuMath::Point2FToPoint2I(EndPoint);
    return DrawLine(PointBitFrameBuffer, StartPointInt, EndPointInt, Color);
}

bool MuRasterizer::DrawLine(MuDevice* Device, const MuPoint3F& StartPoint, const MuPoint3F& EndPoint, const MuRGB& Color)
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
        DrawPoint(Device->GetPointBitFrameBuffer(), MuPoint2I(X0, Y0), Color);
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

bool MuRasterizer::DrawTriangle(FrameBuffer* PointBitFrameBuffer, const MuPoint2F& Point1, const MuPoint2F& Point2, const MuPoint2F& Point3, const MuRGB& Color)
{
    if (DrawLine(PointBitFrameBuffer, Point1, Point2, Color) &&
        DrawLine(PointBitFrameBuffer, Point2, Point3, Color) &&
        DrawLine(PointBitFrameBuffer, Point3, Point1, Color))
    {
        return true;
    }
    return false;
}

bool MuRasterizer::DrawTriangle(MuDevice* Device, const MuPoint3F& Point1, const MuPoint3F& Point2, const MuPoint3F& Point3, const MuRGB& Color)
{
    if (DrawLine(Device, Point1, Point2, Color) &&
        DrawLine(Device, Point2, Point3, Color) &&
        DrawLine(Device, Point3, Point1, Color))
    {
        return true;
    }
    return false;
}

// 中心插值颜色填充
bool MuRasterizer::DrawTriangleSolid(MuDevice* Device, const MuPoint3F& Point1, const MuPoint3F& Point2, const MuPoint3F& Point3)
{
    const float x1 = Point1.x();
    const float y1 = Point1.y();
    const float x2 = Point2.x();
    const float y2 = Point2.y();
    const float x3 = Point3.x();
    const float y3 = Point3.y();
    const float z1 = Point1.z();
    const float z2 = Point2.z();
    const float z3 = Point3.z();
    
    // 计算三角形面积
    const float Area = abs((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2;
    // 计算三角形内部的最小和最大的X和Y坐标
    const int MinY = min(y1, min(y2, y3));
    const int MaxY = max(y1, max(y2, y3));
    const int MinX = min(x1, min(x2, x3));
    const int MaxX = max(x1, max(x2, x3));
    // 遍历三角形内部的每个像素点
    for (int i= MinY; i<= MaxY; i++)
    {
        for (int j= MinX; j<= MaxX; j++)
        {
            // 计算当前点相对于三个顶点的权重值
            const float w1 = ((x2 - j) * (y3 - i) - (x3 - j) * (y2 - i)) / 2 / Area;
            const float w2 = ((x3 - j) * (y1 - i) - (x1 - j) * (y3 - i)) / 2 / Area;
            const float w3 = ((x1 - j) * (y2 - i) - (x2 - j) * (y1 - i)) / 2 / Area;
            // 三个值都为正数时，当前点在三角形内部
            if (w1 < 0 || w2 < 0 || w3 < 0)
            {
                continue;
            }
            // 计算点的重心坐标
            // const float X = w1 * x1 + w2 * x2 + w3 * x3;
            // const float Y = w1 * y1 + w2 * y2 + w3 * y3;
            const float Z = w1 * z1 + w2 * z2 + w3 * z3;

            const MuPoint2I P = MuPoint2I(j, i);
            
            // 获取当前点的深度值
            const float Depth = Device->GetDepth(P);
            
            // 深度测试
            if (Z < Depth)
            {
                continue;
            }
            // 更新深度缓冲区
            {
                Device->SetDepth(P, Z);
            }
            
            
            // 根据权重值和顶点颜色插值得到当前点的颜色
            const int R = abs(w1) * MuColor::Red.x();
            const int G = abs(w2) * MuColor::Green.y();
            const int B = abs(w3) * MuColor::Blue.z();
            const MuRGB Color = MuRGB(R, G, B);
            // 画点
            DrawPoint(Device->GetPointBitFrameBuffer(), P, Color);
        }
    }
    return true;
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

bool MuRasterizer::DrawObj(MuDevice* Device, MuObjModel* ObjModel, const MuRGB& Color)
{
    const auto PointBitFrameBuffer = Device->GetPointBitFrameBuffer();
    if (PointBitFrameBuffer == nullptr)
    {
        return false;
    }
    
    const int FaceCount = ObjModel->GetFaceCount();
    if (FaceCount <= 0)
    {
        return false;
    }

    const FMuObjFace Face = ObjModel->GetFace(0);
    const int ObjFaceVertexCount = Face.GetVertexCount();
    MuLog::LogInfo("DrawObj: Face Vertex Count = %d", ObjFaceVertexCount);

    // 画出obj模型
    for (int i = 0; i < FaceCount; i++)
    {
        const FMuObjFace ObjFace = ObjModel->GetFace(i);
        // 确认 Face 顶点数量
        if (ObjFaceVertexCount == 3)
        {
            const int VertexIndex1 = ObjFace.GetVertex(0).VertexIndex;
            const int VertexIndex2 = ObjFace.GetVertex(1).VertexIndex;
            const int VertexIndex3 = ObjFace.GetVertex(2).VertexIndex;

            MuPoint3F Point1 = ObjModel->GetVertexByIndex(VertexIndex1);
            MuPoint3F Point2 = ObjModel->GetVertexByIndex(VertexIndex2);
            MuPoint3F Point3 = ObjModel->GetVertexByIndex(VertexIndex3);

            const MuPoint3F Point2F1 = MuMath::Point3FToScreenPointWithAspectRatioWithDepth(Point1);
            const MuPoint3F Point2F2 = MuMath::Point3FToScreenPointWithAspectRatioWithDepth(Point2);
            const MuPoint3F Point2F3 = MuMath::Point3FToScreenPointWithAspectRatioWithDepth(Point3);
            
            DrawTriangleSolid(Device, Point2F1, Point2F2, Point2F3);
        }
        else if (ObjFaceVertexCount == 4)
        {
            const int VertexIndex1 = ObjFace.GetVertex(0).VertexIndex;
            const int VertexIndex2 = ObjFace.GetVertex(1).VertexIndex;
            const int VertexIndex3 = ObjFace.GetVertex(2).VertexIndex;
            const int VertexIndex4 = ObjFace.GetVertex(3).VertexIndex;
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
