#pragma once
#include "../Math/MuMath.h"
#include "../Function/MuStruct.h"
#include "MuFunctions.h"

class MuCamera
{
public:
    MuCamera();
    MuCamera(const MuPoint4F& InPosition, const MuVector4F& InLookAtDirection, float InFOV, float InAspectRatio, float InNearPlane, float InFarPlane, EProjectionMode InProjectionMode);

    bool Init();
    
    // 为所有private成员变量生成创建Get和Set方法
    ADD_GET_SET_METHOD(MuPoint4F, CameraPosition)
    ADD_GET_SET_METHOD(MuPoint4F, LookAtPoint)
    ADD_GET_SET_METHOD(MuVector4F, LookAtDirection)
    ADD_GET_SET_METHOD(MuVector4F, UpDirection)
    ADD_GET_SET_METHOD(float, FieldOfView)
    ADD_GET_SET_METHOD(float, AspectRatio)
    ADD_GET_SET_METHOD(float, NearPlane)
    ADD_GET_SET_METHOD(float, FarPlane)
    ADD_GET_SET_METHOD(EProjectionMode, ProjectionMode)
    ADD_GET_SET_METHOD(MuMatrix4F, MVPMatrix)

    // 获取Projection矩阵
    MuMatrix4F GetProjectionMatrix();
    // 获取ViewTransform矩阵
    MuMatrix4F GetViewTransformMatrix();
    
    MuMatrix4F GetPerspectiveTransformMatrix();
    
private:

    /*
     * 相机位置
     */
    MuPoint4F CameraPosition = G_ORIGIN_POINT;

    /*
     * LookAt 点
     */
    MuPoint4F LookAtPoint = G_ORIGIN_POINT;

    /*
     * LookAt 方向
     */
    MuVector4F LookAtDirection = G_NEGATIVE_Z_DIRECTION;

    /*
     * 向上方向
     */
    MuVector4F UpDirection = G_POSITIVE_Y_DIRECTION;
    
    /*
     * FOV (角度制)
     */
    float FieldOfView = 60.f;

    /*
     * 相机宽高比
     */
    float AspectRatio = 1.7f;

    /*
     * 视锥体近平面距离
     */
    float NearPlane = 5.0f;

    /*
     *  视锥体远平面距离
     */
    float FarPlane = 1000.f;

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

    /*
     *  MVPMatrix
     */
    MuMatrix4F MVPMatrix;
};
