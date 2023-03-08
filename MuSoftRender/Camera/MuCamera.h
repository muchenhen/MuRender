#pragma once
#include "../Math/MuMath.h"
#include "../Function/MuStruct.h"

#define ADD_GET_SET_METHOD(ClassName, MemberName) \
    ClassName Get##MemberName() const { return MemberName; } \
    void Set##MemberName(ClassName In##MemberName) { MemberName = In##MemberName; }

class MuCamera
{
public:
    MuCamera();
    MuCamera(const MuPoint4I& InPosition, const MuPoint4I& InLookAtDirection, float InFOV, float InAspectRatio, float InNearPlane, float InFarPlane, EProjectionMode InProjectionMode);

    bool Init();
    
    // 为所有private成员变量生成创建Get和Set方法
    ADD_GET_SET_METHOD(MuPoint4I, CameraPosition)
    ADD_GET_SET_METHOD(MuPoint4I, LookAtPoint)
    ADD_GET_SET_METHOD(MuPoint4I, LookAtDirection)
    ADD_GET_SET_METHOD(MuVector4I, UpDirection)
    ADD_GET_SET_METHOD(float, FOVy)
    ADD_GET_SET_METHOD(float, AspectRatio)
    ADD_GET_SET_METHOD(float, NearPlane)
    ADD_GET_SET_METHOD(float, FarPlane)
    ADD_GET_SET_METHOD(EProjectionMode, ProjectionMode)
    MuMatrix4F GetViewTransformMatrix();
    MuMatrix4F GetPerspectiveTransformMatrix();

private:

    /*
     * 相机位置
     */
    MuPoint4I CameraPosition = G_ORIGIN_POINT;

    /*
     * LookAt 点
     */
    MuPoint4I LookAtPoint = G_ORIGIN_POINT;

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
     * PerspectivTransform 矩阵
     */
    MuMatrix4F PerspectivTransform;

    /*
     * 投影模式
     */
    EProjectionMode ProjectionMode = EProjectionMode::Perspective;
};
