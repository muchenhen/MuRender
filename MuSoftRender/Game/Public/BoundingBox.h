#pragma once

#include "Eigen/Core"
#include <vector>

struct BoundingBox
{
    Eigen::Vector3f Min;
    Eigen::Vector3f Max;

    BoundingBox() :
        Min(Eigen::Vector3f::Constant(std::numeric_limits<float>::max())),
        Max(Eigen::Vector3f::Constant(std::numeric_limits<float>::lowest()))
    {
    }

    void Expand(const Eigen::Vector3f& point)
    {
        Min = Min.cwiseMin(point);
        Max = Max.cwiseMax(point);
    }

    void Expand(const BoundingBox& other)
    {
        Min = Min.cwiseMin(other.Min);
        Max = Max.cwiseMax(other.Max);
    }

    float GetDiagonalLength() const
    {
        return (Max - Min).norm();
    }

    std::vector<V3f> GetCorners() const
    {
        std::vector<V3f> Corners;
        Corners.reserve(8);
        Corners.emplace_back(Min.x(), Min.y(), Min.z());
        Corners.emplace_back(Max.x(), Min.y(), Min.z());
        Corners.emplace_back(Min.x(), Max.y(), Min.z());
        Corners.emplace_back(Max.x(), Max.y(), Min.z());
        Corners.emplace_back(Min.x(), Min.y(), Max.z());
        Corners.emplace_back(Max.x(), Min.y(), Max.z());
        Corners.emplace_back(Min.x(), Max.y(), Max.z());
        Corners.emplace_back(Max.x(), Max.y(), Max.z());
        return Corners;
    }

    Eigen::Vector3f GetCenter() const
    {
        return (Min + Max) / 2.0f;
    }
};