#pragma once
#include "MeshObject.h"
#include "Mesh.h"
#include <memory>

// 使用DirectX 标准的左手坐标系
// X轴指向右边
// Y轴指向上边
// Z轴指向屏幕内

using V3f = Eigen::Vector3f;
using V4f = Eigen::Vector4f;

using M3f = Eigen::Matrix3f;
using M4f = Eigen::Matrix4f;


extern const V3f ORIGIN;
extern const V3f X_AXIS;
extern const V3f Y_AXIS;
extern const V3f Z_AXIS;