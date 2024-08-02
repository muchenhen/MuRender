#pragma once
#pragma warning(disable : 4819)
#include <Eigen/Core>

#include "CoordinateSystem .h"
#pragma warning(default : 4819)
#define M_PI 3.14159265358979323846f

inline Eigen::Vector3f Lerp(const Eigen::Vector3f& start, const Eigen::Vector3f& end, float t)
{
    return start + t * (end - start);
}

inline M4f LookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& center, const Eigen::Vector3f& up)
{
    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f u = up.normalized();
    Eigen::Vector3f s = f.cross(u).normalized();
    u = s.cross(f);

    M4f Result = M4f::Identity();
    Result(0, 0) = s.x();
    Result(1, 0) = s.y();
    Result(2, 0) = s.z();
    Result(0, 1) = u.x();
    Result(1, 1) = u.y();
    Result(2, 1) = u.z();
    Result(0, 2) = -f.x();
    Result(1, 2) = -f.y();
    Result(2, 2) = -f.z();
    Result(0, 3) = -s.dot(eye);
    Result(1, 3) = -u.dot(eye);
    Result(2, 3) = f.dot(eye);
    return Result;
}