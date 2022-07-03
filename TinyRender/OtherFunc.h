#pragma once
#include "Maths.h"

Vec3<int> Vec3fToVec3i(Vec3<float> Vec)
{
    return {static_cast<int>(Vec.x), static_cast<int>(Vec.y), static_cast<int>(Vec.z)};
}
