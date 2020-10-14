/**********************************************
	file:math.h
	插值、矩阵变换、颜色、edgeFunction等函数///
    author：Lcy
	date: 2019-3-27
**********************************************/

#pragma once
#include <windows.h>
#include <algorithm>
#include "Matrix.h"
#include <Eigen/Eigen>

namespace Math_L
{
	extern const float Pi ;
	//MIN
	inline float Min3(const float &a,const float &b,const float &c)
	{
		return std::min<>(a, std::min<>(b, c));
	}
	//Max
	inline float Max3(const float &a, const float &b, const float &c)
	{
		return std::max<>(a, std::max<>(b, c));
	}
	//Clamp
	template<typename T>
	T Clamp(const T &x, const T &min, const T &max)
	{
		return x < min ? min : (x > max ? max : x);
	}
	// Edge function
	inline float EdgeFunc(const Eigen::Vector4f& a, const Eigen::Vector4f& b, const Eigen::Vector4f& p)
	{
		return ((b[0] - a[0])*(p[1] - a[1]) - (b[1] - a[1])*(p[0] - a[0]));
	}

	//degree to radians
	inline float ConvertToRadians(float fDegrees) { return fDegrees * (Pi / 180.0f); }

	
	//平移矩阵
	Eigen::Matrix4f MatrixSetTranslate(float x, float y, float z);

	//缩放矩阵
	Eigen::Matrix4f MatrixSetScale(float x, float y, float z);

	//绕X轴旋转矩阵
	Eigen::Matrix4f MatrixSetRotationX(float angle);

	//绕Y轴旋转矩阵
	Eigen::Matrix4f MatrixSetRotationY(float angle);

	//绕Z轴旋转矩阵
	Eigen::Matrix4f MatrixSetRotationZ(float angle);

	//获取摄像机矩阵或者视角矩阵,view matrix
	Eigen::Matrix4f MatrixSetLookAt(const Eigen::Vector4f& look, const Eigen::Vector4f& at, const Eigen::Vector4f& up);

	//获取投影矩阵  D3DXMatrixPerspectiveFovLH
	Eigen::Matrix4f MatrixPerspectiveFovLH(float fovY, float aspectRatio, float nearZ, float farZ);

	//颜色转化为UINT, argb order
	UINT ColorToUINT(const Eigen::Vector4f& color);

}

class Math
{
};
