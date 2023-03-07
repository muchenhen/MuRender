#pragma once
#include "../Math/MuMath.h"
#include "../Function/MuStruct.h"

class MuCamera
{
public:

    MuCamera(const MuPoint4I& InPosition, const MuPoint4I& InLookAtDirection, float InFOV, float InAspectRatio, float InNearPlane, float InFarPlane, EProjectionMode InProjectionMode);

private:

    /*
     * 相机位置
     */
    MuPoint4I CameraPosition = G_ORIGIN_POINT;

    /*
     * LookAt 方向
     */
    MuPoint4I LookAtDirection = G_NEGATIVE_Z_DIRECTION;

    /*
     * 向上方向
     */
    MuVector4I UpDirection = G_POSITIVE_Y_DIRECTION;
    
    /*
     * FOV y (角度制
     */
    float FOVy = 60.f;

    /*
     * 相机宽高比
     */
    float AspectRatio = 1.7f;

    /*
     * 视锥体近平面距离
     */
    float NearPlane = 0.1f;

    /*
     *  视锥体远平面距离
     */
    float FarPlane = 1000.f;

    /*
     * ViewTransform 矩阵
     */
    MuMatrix4F ViewTransform;

    /*
     * 投影模式
     */
    EProjectionMode ProjectionMode = EProjectionMode::Perspective;
};
