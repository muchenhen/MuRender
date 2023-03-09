#pragma once
#include "../Math/MuMath.h"
#include "../Function/MuStruct.h"

#define ADD_GET_SET_METHOD(ClassName, MemberName) \
    ClassName Get##MemberName() const { return MemberName; } \
    void Set##MemberName(ClassName In##MemberName) { MemberName = In##MemberName; }

/*
 * 摄像机类
 * 参考ue4的UCameraComponent
 */
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
    ADD_GET_SET_METHOD(float, AspectRatio)
    ADD_GET_SET_METHOD(float, NearPlane)
    ADD_GET_SET_METHOD(float, FarPlane)
    ADD_GET_SET_METHOD(EProjectionMode, ProjectionMode)
    MuMatrix4F GetViewTransformMatrix();
    MuMatrix4F GetPerspectiveTransformMatrix();

private:
    /*
     * FOV 角度制
     */
    float FieldOfView;

    /*
     * 正交视图的所需宽度（以世界单位表示）（在透视模式中忽略
     */
    float OrthoWidth;

    /*
     * 正交视图的近平面距离（世界单位）
     */
    float OrthoNearClipPlane;

    /*
     * 正交视图的远平面距离（世界单位）
     */
    float OrthoFarClipPlane;

    /*
     * 相机宽高比
     */
    float AspectRatio;
    
    /*
     * 相机位置
     */
    MuPoint4I CameraPosition;

    /*
     * LookAt 点
     */
    MuPoint4I LookAtPoint;

    /*
     * LookAt 方向
     */
    MuPoint4I LookAtDirection;

    /*
     * 向上方向
     */
    MuVector4I UpDirection;
    
    

    

    /*
     * 视锥体近平面距离
     */
    float NearPlane;

    /*
     *  视锥体远平面距离
     */
    float FarPlane;

    /*
     * ViewTransform 矩阵
     */
    MuMatrix4F ViewTransform;

    /*
     * PerspectiveTransform 矩阵
     */
    MuMatrix4F PerspectiveTransform;

    /*
     * 投影模式
     */
    EProjectionMode ProjectionMode = EProjectionMode::Perspective;
};
