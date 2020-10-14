/**********************************************
	file:math.h
	��ֵ������任����ɫ��edgeFunction�Ⱥ���///
    author��Lcy
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

	
	//ƽ�ƾ���
	Eigen::Matrix4f MatrixSetTranslate(float x, float y, float z);

	//���ž���
	Eigen::Matrix4f MatrixSetScale(float x, float y, float z);

	//��X����ת����
	Eigen::Matrix4f MatrixSetRotationX(float angle);

	//��Y����ת����
	Eigen::Matrix4f MatrixSetRotationY(float angle);

	//��Z����ת����
	Eigen::Matrix4f MatrixSetRotationZ(float angle);

	//��ȡ�������������ӽǾ���,view matrix
	Eigen::Matrix4f MatrixSetLookAt(const Eigen::Vector4f& look, const Eigen::Vector4f& at, const Eigen::Vector4f& up);

	//��ȡͶӰ����  D3DXMatrixPerspectiveFovLH
	Eigen::Matrix4f MatrixPerspectiveFovLH(float fovY, float aspectRatio, float nearZ, float farZ);

	//��ɫת��ΪUINT, argb order
	UINT ColorToUINT(const Eigen::Vector4f& color);

}

class Math
{
};
