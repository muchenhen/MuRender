#include "Object.h"

Eigen::Matrix3f Object::GetNormalMatrix() const
{
    return GetModelMatrix().block<3, 3>(0, 0).inverse().transpose();
}