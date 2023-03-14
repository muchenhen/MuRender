#pragma once

#define FrameBuffer unsigned int
#define FrameZBuffer float
#include "MuFunctions.h"
#include "MuMath.h"

enum class EMuRenderMode
{
	// 线框
    Wireframe,
	// 随机颜色填充
    Color,
	// 贴图
	Texture
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

    ADD_GET_SET_METHOD(EMuRenderMode, RenderMode);

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
