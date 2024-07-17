#pragma once

#include <Eigen/Core>

Eigen::Vector3f Lerp(const Eigen::Vector3f& start, const Eigen::Vector3f& end, float t)
{
    return start + t * (end - start);
}