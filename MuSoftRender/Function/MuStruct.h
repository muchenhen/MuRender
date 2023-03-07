#pragma once

// 投影模式
enum class EProjectionMode
{
    // 正交投影
    Orthographic,
    // 透视投影
    Perspective
};

/*
 * 使用右手坐标系 与Games101一致
 */

const MuPoint4I G_ORIGIN_POINT = MuPoint4I(0, 0, 0, 1);

// 负Z方向
const MuPoint4I G_NEGATIVE_Z_DIRECTION = MuPoint4I(0, 0, -1, 0);
// 负Y方向
const MuPoint4I G_NEGATIVE_Y_DIRECTION = MuPoint4I(0, -1, 0, 0);
// 负X方向
const MuPoint4I G_NEGATIVE_X_DIRECTION = MuPoint4I(-1, 0, 0, 0);
// 正Z方向
const MuPoint4I G_POSITIVE_Z_DIRECTION = MuPoint4I(0, 0, 1, 0);
// 正Y方向
const MuPoint4I G_POSITIVE_Y_DIRECTION = MuPoint4I(0, 1, 0, 0);
// 正X方向
const MuPoint4I G_POSITIVE_X_DIRECTION = MuPoint4I(1, 0, 0, 0);

struct FMuObjFace
{
    
};
