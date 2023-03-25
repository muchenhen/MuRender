#pragma once
#include "MuMath.h"

MuPoint3F MuMath::NDCtoScreen(const MuPoint3F& Point)
{
    constexpr float AspectRatio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    const float x = (Point.x() + 1) * SCREEN_WIDTH / 2;
    const float y = (Point.y() + 1) * SCREEN_HEIGHT / 2;
    
    MuPoint3F ScreenPoint;
    if (AspectRatio < 1.0f)
    {
        ScreenPoint = MuPoint3F(x, y * AspectRatio, Point.z());
        return ScreenPoint;
    }
    else
    {
        ScreenPoint = MuPoint3F(x / AspectRatio, y, Point.z());
        return ScreenPoint;
    }
}

MuVector3F MuMath::ComputeTriangleNormal(const MuPoint3F& Point1, const MuPoint3F& Point2, const MuPoint3F& Point3)
{
    const MuVector3F Edge1 = Point2 - Point1;
    const MuVector3F Edge2 = Point3 - Point1;
    return Edge1.cross(Edge2).normalized();
}

bool MuMath::BackFaceCulling(const MuPoint3F& Point1, const MuPoint3F& Point2, const MuPoint3F& Point3, const MuVector3F& CameraDirection)
{
    const MuVector3F Normal = ComputeTriangleNormal(Point1, Point2, Point3);
    return Normal.dot(CameraDirection) < 0;
}

MuMatrix4F MuMath::GetOrthographicProjectionMatrix(float FieldOfView, float Aspect, float Near, float Far)
{
    const float HalfWidth = FieldOfView / 2;
    const float HalfHeight = HalfWidth / Aspect;
    const float Width = HalfWidth * 2;
    const float Height = HalfHeight * 2;
    const float Depth = Far - Near;

    MuMatrix4F OrthogonalMatrix;
    OrthogonalMatrix << 2 / Width, 0, 0, 0,
        0, 2 / Height, 0, 0,
        0, 0, 2 / Depth, 0,
        0, 0, 0, 1;

    return OrthogonalMatrix;
}

MuMatrix4F MuMath::GetPerspectiveProjectionMatrix(float FieldOfView, float Aspect, float Near, float Far)
{
    const float HalfWidth = FieldOfView / 2;
    const float HalfHeight = HalfWidth / Aspect;
    const float Width = HalfWidth * 2;
    const float Height = HalfHeight * 2;
    const float Depth = Far - Near;

    MuMatrix4F PerspectiveMatrix;
    PerspectiveMatrix << 2 * Near / Width, 0, 0, 0,
        0, 2 * Near / Height, 0, 0,
        0, 0, -(Far + Near) / Depth, -2 * Far * Near / Depth,
        0, 0, -1, 0;

    return PerspectiveMatrix;
}
