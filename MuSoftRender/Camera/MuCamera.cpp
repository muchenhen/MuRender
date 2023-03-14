#include "MuCamera.h"

MuCamera::MuCamera()
= default;

MuCamera::MuCamera(const MuPoint4F& InPosition, const MuVector4F& InLookAtDirection, float InFOV, float InAspectRatio, float InNearPlane, float InFarPlane, EProjectionMode InProjectionMode)
{
    CameraPosition = InPosition;
    LookAtDirection = InLookAtDirection;
    FOVy = InFOV;
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
    if (FOVy < 0.1f || FOVy > 180.f)
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
    // PerspectivTransform = MuMath::GetPerspectiveProjectionMatrix(FOVy, AspectRatio, NearPlane, FarPlane);
    
    return true;
}

MuMatrix4F MuCamera::GetViewTransformMatrix()
{
    return ViewTransform;
}

MuMatrix4F MuCamera::GetPerspectiveTransformMatrix()
{
    return PerspectivTransform;
}
