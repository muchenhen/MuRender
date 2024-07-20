#pragma once
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)
#define M_PI 3.14159265358979323846f

inline Eigen::Vector3f Lerp(const Eigen::Vector3f& start, const Eigen::Vector3f& end, float t)
{
    return start + t * (end - start);
}