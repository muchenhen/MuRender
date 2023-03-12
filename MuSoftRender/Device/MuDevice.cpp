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
    RenderMode()
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
        ZBuffer = new FrameZBuffer[InWidth * InHeight]{-1000.0f};
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
