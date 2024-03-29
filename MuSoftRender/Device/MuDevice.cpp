#include "MuDevice.h"

MuDevice::MuDevice(unsigned* PointBitFrameBuffer, int Width, int Height, EMuRenderMode RenderMode):
    PointBitFrameBuffer(PointBitFrameBuffer),
    ZBuffer(nullptr),
    Width(Width),
    Height(Height),
    RenderMode(RenderMode)
{
}

MuDevice::MuDevice():
    PointBitFrameBuffer(nullptr),
    ZBuffer(nullptr),
    Width(0),
    Height(0),
    RenderMode(EMuRenderMode::Wireframe)
{
}

MuDevice::~MuDevice()
{
    PointBitFrameBuffer = nullptr;
}

bool MuDevice::InitDevice(void* InPointBitFrameBuffer, int InWidth, int InHeight, EMuRenderMode InRenderMode)
{
    if (! InPointBitFrameBuffer)
    {
        return false;
    }
    
    if (ZBuffer == nullptr)
    {
        ZBuffer = new FrameZBuffer[InWidth * InHeight];
        // 遍历ZBuffer，将所有深度值设置为-1000
        for (int i = 0; i < InWidth * InHeight; i++)
        {
            ZBuffer[i] = -1000.0f;
        }
    }
    
    PointBitFrameBuffer = static_cast<FrameBuffer*>(InPointBitFrameBuffer);
    Width = InWidth;
    Height = InHeight;
    RenderMode = InRenderMode;
    return true;
}

FrameBuffer* MuDevice::GetPointBitFrameBuffer()
{
    return PointBitFrameBuffer;
}

FrameZBuffer* MuDevice::GetZBuffer()
{
    return ZBuffer;
}

float MuDevice::GetDepth(MuPoint2I Point)
{
    // 确认点的坐标在ZBuffer的范围内
    if (Point.x() < 0 || Point.x() >= Width || Point.y() < 0 || Point.y() >= Height)
    {
        return -1000.0f;
    }
    const float Z = ZBuffer[Point.x() + Point.y() * Width];
    // 获取对应点的深度
    return Z;
}

void MuDevice::SetDepth(MuPoint2I Point, float Z)
{
    // 确认点的坐标在ZBuffer的范围内
    if (Point.x() < 0 || Point.x() >= Width || Point.y() < 0 || Point.y() >= Height)
    {
        return;
    }
    // 设置对应点的深度
    ZBuffer[Point.x() + Point.y() * Width] = Z;
}
