#include "MuMath.h"

MuPoint3F MuMath::NDCtoScreen(const MuPoint3F& Point)
{
    // 屏幕宽高比
    constexpr float AspectRatio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    float WidthMap = static_cast<float>(SCREEN_WIDTH) / 2;
    float HeightMap = static_cast<float>(SCREEN_HEIGHT) / 2;
    constexpr float WidthOffset = static_cast<float>(SCREEN_WIDTH) / 2;
    constexpr float HeightOffset = static_cast<float>(SCREEN_HEIGHT) / 2;
    if (AspectRatio > 0)
    {
        WidthMap = WidthMap / AspectRatio;
    }
    else
    {
        HeightMap = HeightMap * AspectRatio;
    }
    // 将NDC坐标转换为屏幕坐标
    const float x = Point.x() * WidthMap + WidthOffset;
    const float y = Point.y() * HeightMap + HeightOffset;

    MuPoint3F ScreenPoint;
    ScreenPoint << x, y, Point.z();
    return ScreenPoint;
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

/**
 * \brief 计算摄像机的正交投影矩阵
 * \param Width 正交投影的视锥宽度
 * \param Aspect 宽高比
 * \param Near 近平面距离
 * \param Far 远平面距离
 * \return 摄像机的正交投影矩阵
 */
MuMatrix4F MuMath::GetOrthographicProjectionMatrix(const float Aspect, const float Width, const float Near, const float Far)
{
    MuMatrix4F OrthogonalMatrix = MuMatrix4F::Identity();
    OrthogonalMatrix(0, 0) = 2.0f / Width;
    OrthogonalMatrix(1, 1) = 2.0f / (Width * Aspect);
    OrthogonalMatrix(2, 2) = -2.0f / (Far - Near);
    OrthogonalMatrix(3, 0) = -1.0f;
    OrthogonalMatrix(3, 1) = -1.0f / Aspect;
    OrthogonalMatrix(3, 2) = -(Far + Near) / (Far - Near);
    return OrthogonalMatrix;
}

MuMatrix4F MuMath::GetPerspectiveProjectionMatrix(const float FieldOfView, const float Aspect, const float Near, const float Far)
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
