#pragma once
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <algorithm>

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

}

namespace MuColor
{

inline unsigned int MuRGBtoUInt(const MuRGB& Color)
{
    int r = 255 * Color.x();
    int g = 255 * Color.y();
    int b = 255 * Color.z();

    r = MuMath::Clamp(r, 0, 255);
    g = MuMath::Clamp(g, 0, 255);
    b = MuMath::Clamp(b, 0, 255);
    return static_cast<unsigned int>((r << 16) | (g << 8) | (b));
}

const MuRGB White(1, 1, 1);
;
}
