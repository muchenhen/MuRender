#include "MuDevice.h"

MuDevice::MuDevice(unsigned* PointBitFrameBuffer, int Width, int Height, EMuRenderMode RenderMode):
    PointBitFrameBuffer(PointBitFrameBuffer),
    Width(Width),
    Height(Height),
    RenderMode(RenderMode)
{
}

MuDevice::MuDevice():
    PointBitFrameBuffer(nullptr),
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
