#pragma once
#include "../eigen-3.4.0/eigen-3.4.0/Eigen/Eigen"
#include "../eigen-3.4.0/eigen-3.4.0/Eigen/Dense"
#include <algorithm>
#include <random>

// 屏幕宽度
constexpr int SCREEN_WIDTH = 1280;
// 屏幕高度
constexpr int SCREEN_HEIGHT = 720;
constexpr float PI = 3.14159265358979323846f;

typedef Eigen::Vector2i MuPoint2I;
typedef Eigen::Vector3i MuPoint3I;
typedef Eigen::Vector4i MuPoint4I;

typedef Eigen::Vector2f MuPoint2F;
typedef Eigen::Vector3f MuPoint3F;
typedef Eigen::Vector4f MuPoint4F;

typedef Eigen::Vector2i MuVector2I;
typedef Eigen::Vector3i MuVector3I;
typedef Eigen::Vector4i MuVector4I;

typedef Eigen::Vector2f MuVector2F;
typedef Eigen::Vector3f MuVector3F;
typedef Eigen::Vector4f MuVector4F;

typedef Eigen::Vector3i MuRGB;
typedef Eigen::Vector4i MuRGBA;

typedef Eigen::Matrix4f MuMatrix4F;

namespace MuMath
{
static bool bUseRodrigues = false;

template <typename T>
T Clamp(T Value, T Min, T Max)
{
    return (std::min)((std::max)(Value, Min), Max);
}

// 生成指定范围内的随机数
inline int RandomInt(int Min, int Max)
{
    std::random_device RandomDevice;
    std::mt19937 RandomEngine(RandomDevice());
    std::uniform_int_distribution<int> Distribution(Min, Max);
    return Distribution(RandomEngine);
}

// MuPoint3F -> MuPoint2F
inline MuPoint2F Point3FToPoint2F(const MuPoint3F& Point)
{
    return MuPoint2F(Point.x(), Point.y());
}

// MuPoint3I -> MuPoint2I
inline MuPoint2I Point3IToPoint2I(const MuPoint3I& Point)
{
    return MuPoint2I(Point.x(), Point.y());
}

// MuPoint4I -> MuPoint3I
inline MuPoint3I Point4IToPoint3I(const MuPoint4I& Point)
{
       return MuPoint3I(Point.x(), Point.y(), Point.z());
}

// MuPoint4I -> MuPoint2I
inline MuPoint2I Point4IToPoint2I(const MuPoint4I& Point)
{
       return MuPoint2I(Point.x(), Point.y());
}

// Point2FToPoint2I
inline MuPoint2I Point2FToPoint2I(const MuPoint2F& Point)
{
    return MuPoint2I(Point.x(), Point.y());
}

// 将[-1,1]范围的点 映射到 [0, SCREEN_WIDTH] 和 [0, SCREEN_HEIGHT] 范围内
inline MuPoint2I Point3IToScreenPoint(const MuPoint3I& Point)
{
    return MuPoint2I((Point.x() + 1) * SCREEN_WIDTH / 2, (Point.y() + 1) * SCREEN_HEIGHT / 2);
}

inline MuPoint2I Point3FToScreenPoint(const MuPoint3F& Point)
{
    return MuPoint2I((Point.x() + 1) * SCREEN_WIDTH / 2, (Point.y() + 1) * SCREEN_HEIGHT / 2);
}

// 将[-1,1]范围的点 映射到 [0, SCREEN_WIDTH] 和 [0, SCREEN_HEIGHT] 范围内 但是要保持宽高比不变
// 并且 按照宽高比较大的那个方向进行缩放
inline MuPoint2F Point3FToScreenPointWithAspectRatio(const MuPoint3F& Point)
{
    const float AspectRatio = SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    const float x = (Point.x() + 1) * SCREEN_WIDTH / 2;
    const float y = (Point.y() + 1) * SCREEN_HEIGHT / 2;
    MuPoint2F ScreenPoint;
    if (AspectRatio < 1.0f)
    {
        ScreenPoint = MuPoint2F(x, y * AspectRatio);
        return ScreenPoint;
    }
    else
    {
        ScreenPoint = MuPoint2F(x / AspectRatio, y);
        return ScreenPoint;
    }
}

// 将[-1,1]范围的点 映射到 [0, SCREEN_WIDTH] 和 [0, SCREEN_HEIGHT] 范围内 但是要保持宽高比不变
// Z值保留 保持宽高比
inline MuPoint3F Point3FToScreenPointWithAspectRatioWithDepth(const MuPoint3F& Point)
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

/*
 * Model Transform Matrix
 * 模型变换相关矩阵
 */

// ----------------- 线性变换 -----------------

// 绕X轴旋转矩阵
inline MuMatrix4F GetRotateXMatrix(float Angle)
{
    // 角度转弧度
    Angle = Angle * PI / 180.0f;
    const float Cos = std::cos(Angle);
    const float Sin = std::sin(Angle);
    MuMatrix4F RotateXMatrix;
    RotateXMatrix << 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, Cos, -Sin, 0.0f,
        0.0f, Sin, Cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    return RotateXMatrix;
}

// 绕Y轴旋转矩阵
inline MuMatrix4F GetRotateYMatrix(float Angle)
{
    // 角度转弧度
    Angle = Angle * PI / 180.0f;
    const float Cos = std::cos(Angle);
    const float Sin = std::sin(Angle);
    MuMatrix4F RotateYMatrix;
    RotateYMatrix << Cos, 0.0f, Sin, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -Sin, 0.0f, Cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    return RotateYMatrix;
}

// 绕Z轴旋转矩阵
inline MuMatrix4F GetRotateZMatrix(float Angle)
{
    // 角度转弧度
    Angle = Angle * PI / 180.0f;
    const float Cos = std::cos(Angle);
    const float Sin = std::sin(Angle);
    MuMatrix4F RotateZMatrix;
    RotateZMatrix << Cos, -Sin, 0.0f, 0.0f,
        Sin, Cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    return RotateZMatrix;
}

// 缩放矩阵
inline MuMatrix4F GetScaleMatrix(float Scale)
{
    MuMatrix4F ScaleMatrix;
    ScaleMatrix << Scale, 0.0f, 0.0f, 0.0f,
        0.0f, Scale, 0.0f, 0.0f,
        0.0f, 0.0f, Scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f;
    return ScaleMatrix;
}

// ----------------- 非线性变换 -----------------

// 平移矩阵
inline MuMatrix4F GetTranslateMatrix(float X, float Y, float Z)
{
    MuMatrix4F TranslateMatrix;
    TranslateMatrix << 1.0f, 0.0f, 0.0f, X,
        0.0f, 1.0f, 0.0f, Y,
        0.0f, 0.0f, 1.0f, Z,
        0.0f, 0.0f, 0.0f, 1.0f;
    return TranslateMatrix;
}

/*
 * View Transform Matrix
 *
 * 对包括相机在内的所有物体进行Transform
 * 目标是将相机移动到原点，且相机面向Z轴负方向 其他物体做相同的变换 这样和相机的相对位置不变
 * 根据相机的当前位置、朝向、up方向，计算出相机的Model Transform矩阵
 */
inline MuMatrix4F GetViewTransformMatrix(const MuPoint4F& CameraPosition, const MuPoint4F& CameraLookAt, const MuPoint4F& CameraUp)
{
    // 计算相机的平移矩阵
    const MuMatrix4F CameraTranslationMatrix = GetTranslateMatrix(-CameraPosition.x(), -CameraPosition.y(), -CameraPosition.z());

    MuMatrix4F CameraRotationMatrix;
    
    // 分别计算相机绕Z、Y、X轴旋转的角度
    if(!bUseRodrigues)
    {
        // 相机绕Z轴旋转的角度
        const float CameraRotateZAngle = std::atan2(CameraLookAt.y(), CameraLookAt.x()) * 180.0f / PI;
        // 相机绕Y轴旋转的角度
        const float CameraRotateYAngle = std::atan2(CameraLookAt.z(), std::sqrt(CameraLookAt.x() * CameraLookAt.x() + CameraLookAt.y() * CameraLookAt.y())) * 180.0f / PI;
        // 相机绕X轴旋转的角度
        const float CameraRotateXAngle = std::atan2(CameraUp.z(), CameraUp.y()) * 180.0f / PI;
        // 计算相机的旋转矩阵
        CameraRotationMatrix = GetRotateZMatrix(CameraRotateZAngle) * GetRotateYMatrix(CameraRotateYAngle) * GetRotateXMatrix(CameraRotateXAngle);
    }
    else
    {
        MuPoint3F CameraPosition3F = MuPoint3F(CameraPosition.x(), CameraPosition.y(), CameraPosition.z());
        MuPoint3F CameraLookAt3F =  MuPoint3F(CameraLookAt.x(), CameraLookAt.y(), CameraLookAt.z());
        MuPoint3F CameraUp3F =  MuPoint3F(CameraUp.x(), CameraUp.y(), CameraUp.z());

        const MuPoint3F CameraZAxis = (CameraPosition3F - CameraLookAt3F).normalized();
        const MuPoint3F CameraXAxis = CameraUp3F.cross(CameraZAxis).normalized();
        const MuPoint3F CameraYAxis = CameraZAxis.cross(CameraXAxis).normalized();
        // // 计算相机的Z轴方向
        // const MuPoint4F CameraZAxis = (CameraPosition - CameraLookAt).normalized();
        //
        // // 计算相机的X轴方向
        // const MuPoint4F CameraXAxis = CameraUp.cross(CameraZAxis).normalized();
        //
        // // 计算相机的Y轴方向
        // const MuPoint4F CameraYAxis = CameraZAxis.cross(CameraXAxis).normalized();
        
        // 计算相机的旋转矩阵
        CameraRotationMatrix << CameraXAxis.x(), CameraYAxis.x(), CameraZAxis.x(), 0.0f,
            CameraXAxis.y(), CameraYAxis.y(), CameraZAxis.y(), 0.0f,
            CameraXAxis.z(), CameraYAxis.z(), CameraZAxis.z(), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;
    }

    // 相机的Model Transform矩阵
    return CameraRotationMatrix * CameraTranslationMatrix;
}

/*
 * Projection Transform Matrix
 * 投影变换矩阵
 */

// 正交投影矩阵



// 透视投影矩阵
/*
 * 通过摄像机FOVy、宽高比、近平面距离、远平面距离，计算出透视投影矩阵
 */
inline MuMatrix4F GetPerspectiveProjectionMatrix(float FOVy, float Aspect, float Near, float Far)
{
    // 角度转弧度
    FOVy = FOVy * PI / 180.0f;
    // 计算焦距
    const float FocalLength = 1.0f / std::tan(FOVy / 2.0f);
    // 计算透视投影矩阵
    MuMatrix4F PerspectiveProjectionMatrix;
    PerspectiveProjectionMatrix << FocalLength / Aspect, 0.0f, 0.0f, 0.0f,
        0.0f, FocalLength, 0.0f, 0.0f,
        0.0f, 0.0f, (Far + Near) / (Near - Far), 2.0f * Far * Near / (Near - Far),
        0.0f, 0.0f, -1.0f, 0.0f;
    return PerspectiveProjectionMatrix;
}

}

namespace MuColor
{
inline unsigned int HexRGBtoUInt(const std::string& HexString)
{
    // 将字符串转换为16进制整数
    const unsigned int HexValue = std::stoi(HexString, nullptr, 16);

    // 将R、G、B通道的值提取出来，并将其缩放为0到255之间的整数
    const unsigned int R = (HexValue >> 16) & 0xFF;
    const unsigned int G = (HexValue >> 8) & 0xFF;
    const unsigned int B = HexValue & 0xFF;

    // 将三个通道的值组合成一个32位的unsigned int
    return (R << 16) | (G << 8) | B;
}

inline unsigned int MuRGBtoUInt(const MuRGB& Color)
{
    int R = Color.x();
    int G = Color.y();
    int B = Color.z();

    R = MuMath::Clamp(R, 0, 255);
    G = MuMath::Clamp(G, 0, 255);
    B = MuMath::Clamp(B, 0, 255);
    return static_cast<unsigned int>((R << 16) | (G << 8) | (B));
}

inline MuRGB GetRandomMuRGB()
{
    const int R = MuMath::RandomInt(0, 255);
    const int G = MuMath::RandomInt(0, 255);
    const int B = MuMath::RandomInt(0, 255);

    return MuRGB{R, G, B};
}

inline unsigned int GetRandomColor()
{
    const int R = MuMath::RandomInt(0, 255);
    const int G = MuMath::RandomInt(0, 255);
    const int B = MuMath::RandomInt(0, 255);

    return static_cast<unsigned int>((R << 16) | (G << 8) | (B));
}

const MuRGB White(255, 255, 255);
const MuRGB Red(255, 0, 0);
const MuRGB Green(0, 255, 0);
const MuRGB Blue(0, 0, 255);
const MuRGB Black(0, 0, 0);
const MuRGB Yellow(255, 255, 0);
const MuRGB Cyan(0, 255, 255);
const MuRGB Magenta(255, 0, 255);
const MuRGB Gray(128, 128, 128);
const MuRGB DarkGray(64, 64, 64);
const MuRGB LightGray(192, 192, 192);
const MuRGB Orange(255, 128, 0);
const MuRGB Purple(128, 0, 128);
const MuRGB Brown(128, 64, 0);
const MuRGB Pink(255, 128, 128);
const MuRGB LightBlue(128, 128, 255);
const MuRGB DarkGreen(0, 128, 0);
const MuRGB DarkRed(128, 0, 0);
const MuRGB DarkBlue(0, 0, 128);
const MuRGB DarkCyan(0, 128, 128);
const MuRGB DarkMagenta(128, 0, 128);
const MuRGB DarkYellow(128, 128, 0);
const MuRGB LightGreen(128, 255, 128);
const MuRGB LightRed(255, 128, 128);
const MuRGB LightCyan(128, 255, 255);
const MuRGB LightMagenta(255, 128, 255);
const MuRGB LightYellow(255, 255, 128);
}
