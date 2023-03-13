#pragma once

#define FrameBuffer unsigned int
#define FrameZBuffer float
#include "MuMath.h"

enum class EMuRenderMode
{
	// 线框
    wireframe,
	// 随机颜色填充
    color,
	// 贴图
	texture
};

class MuDevice
{
public:
    MuDevice(FrameBuffer* PointBitFrameBuffer, int Width, int Height, EMuRenderMode RenderMode);

    MuDevice();
    ~MuDevice();

	bool InitDevice(void* InPointBitFrameBuffer, int InWidth, int InHeight, EMuRenderMode InRenderMode);

	FrameBuffer* GetPointBitFrameBuffer();

    FrameZBuffer* GetZBuffer();
    float GetDepth(MuPoint2I Point);
    void SetDepth(MuPoint2I Point, float Z);

    // void Tick();

private:
	// 存储位图信息 最后绘制完成的像素会保存在这个地址，用于win api绘制
    FrameBuffer* PointBitFrameBuffer;

    // 存储深度信息
    FrameZBuffer* ZBuffer;

	// 窗口宽度
	int Width;

	// 窗口高度
    int Height;

	// 渲染模式
    EMuRenderMode RenderMode;

};
