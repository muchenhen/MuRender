#pragma once
#include <algorithm>
#include <Windows.h>
#include "Matrix.h"
#include "Vector.h"

constexpr float PI = 3.14159265f;

namespace MathSet
{

    float lerp(float x1, float x2, float t);

    MuVector lerp(const MuVector& v1, const MuVector& v2, float t);

    MuVector3D lerp(const MuVector3D& v1, const MuVector3D& v2, float t);

    MuVector2D lerp(const MuVector2D& v1, const MuVector2D& v2, float t);

    template <typename T>
    T clamp(T x, T min, T max)
    {
        if (x <= min)
            return min;
        else if (x >= max)
            return max;
        return x;
    }

    float max3(const float& a, const float& b, const float& c);

    float min3(const float& a, const float& b, const float& c);

    float convertToRadians(float fDegrees);

    Matrix unitMatrix();

    Matrix transposedMatrix(const Matrix& m);

    float det(const Matrix& m);

    float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3);

    Matrix adjMatrix(const Matrix& m);

    Matrix inverseMatrix(const Matrix& m);

    Matrix scaleMatrix(float scaleX, float scaleY, float scaleZ);

    Matrix translationMatrix(float transX, float transY, float transZ);

    Matrix rotationX(float a);

    Matrix rotationY(float a);

    Matrix rotationZ(float a);

    Matrix rotateV(const MuVector3D& v, float a);

    Matrix MatrixLookAtLh(const MuVector& eyePosizition, const MuVector& FocusPosition, const MuVector& UpDirection);

    Matrix MatrixPerspectiveFovLH(const float& FovAngleY, const float& AspectRatio, const float& NearZ, const float& FarZ);

    unsigned int color2UINT(const MuVector& color);

    MuVector multVector4(const MuVector& v, const Matrix& m);

    float edgeFunc(const MuVector& a, const MuVector& b, const MuVector& p);
} // namespace MathSet