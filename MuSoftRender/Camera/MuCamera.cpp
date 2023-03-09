#include "MuCamera.h"

MuCamera::MuCamera():
    NearPlane(0),
    FarPlane(0)
{
    FieldOfView = 90.0f;
    AspectRatio = 1.777778f;
    OrthoWidth = 512.0f;
    OrthoNearClipPlane = 0.0f;
    OrthoFarClipPlane = 1000.0f;
}

MuCamera::MuCamera(const MuPoint4I& InPosition, const MuPoint4I& InLookAtDirection, float InFOV, float InAspectRatio, float InNearPlane, float InFarPlane, EProjectionMode InProjectionMode):
    OrthoWidth(512.0f),
    OrthoNearClipPlane(0.0f),
    OrthoFarClipPlane(1000.0f)
{
    CameraPosition = InPosition;
    LookAtDirection = InLookAtDirection;
    FieldOfView = InFOV;
    AspectRatio = InAspectRatio;
    NearPlane = InNearPlane;
    FarPlane = InFarPlane;
    ProjectionMode = InProjectionMode;
}

bool MuCamera::Init()
{
    // 确认相机位置和LookAt方向不是同一个点
    if (CameraPosition == LookAtDirection)
    {
        return false;
    }
    // 确认fov y在0.1-180度之间
    if (FieldOfView < 0.1f || FieldOfView > 180.f)
    {
        return false;
    }
    // 确认宽高比在1-2.5之间
    if (AspectRatio < 1.f || AspectRatio > 2.5f)
    {
        return false;
    }
    // 确认远平面比近平面远
    if (FarPlane <= NearPlane)
    {
        return false;
    }

    // 计算LookAt方向
    LookAtDirection = LookAtDirection - CameraPosition;
    LookAtDirection.normalize();

    // ViewTransform = MuMath::GetViewTransformMatrix(CameraPosition, LookAtDirection, UpDirection);
    // PerspectiveTransform = MuMath::GetPerspectiveProjectionMatrix(FieldOfView, AspectRatio, NearPlane, FarPlane);
    
    return true;
}

MuMatrix4F MuCamera::GetViewTransformMatrix()
{
    return ViewTransform;
}

MuMatrix4F MuCamera::GetPerspectiveTransformMatrix()
{
    return PerspectiveTransform;
}
