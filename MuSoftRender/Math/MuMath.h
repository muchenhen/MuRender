#pragma once
#include "../eigen-3.4.0/eigen-3.4.0/Eigen/Eigen"
#include "../eigen-3.4.0/eigen-3.4.0/Eigen/Dense"
#include <algorithm>
#include <random>

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
constexpr float PI = 3.14159265358979323846f;

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

// MuPoint3I -> MuPoint2I
inline MuPoint2I Point3IToPoint2I(const MuPoint3I& Point)
{
    return MuPoint2I(Point.x(), Point.y());
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
/*
 * 已知摄像机位置 方向 up方向 FieldOfView OrthoWidth OrthoNearClipPlane OrthoFarClipPlane 计算出摄像机的平截头体
 */
 // TODO:


// 确定了正交投影的平截头体，分别是从摄像机看 左面 右面 下面 上面 近平面 远平面
inline MuMatrix4F GetOrthoProjectionMatrix(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    MuMatrix4F OrthoProjectionMatrix;
    OrthoProjectionMatrix <<
        2.0f / (Right - Left), 0.0f,                  0.0f,                -(Right + Left) / (Right - Left),
        0.0f,                  2.0f / (Top - Bottom), 0.0f,                -(Top + Bottom) / (Top - Bottom),
        0.0f,                  0.0f,                  2.0f / (Near - Far), -(Near + Far) / (Near - Far),
        0.0f,                  0.0f,                  0.0f,                1.0f;
    return OrthoProjectionMatrix;
}

// 透视投影矩阵
/*
 * 通过摄像机FieldOfView、宽高比、近平面距离、远平面距离，计算出透视投影矩阵
 */
inline MuMatrix4F GetPerspectiveProjectionMatrix(float FieldOfView, float AspectRatio, float Near, float Far)
{
   
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
