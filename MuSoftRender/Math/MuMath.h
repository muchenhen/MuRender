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

namespace MuMath
{
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

    return MuRGB{R,G,B};
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
