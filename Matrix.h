#pragma once
#include "VectorL.h"

struct Matrix4
{
	union
	{
		float m[4][4];
		struct
		{
			float  _11; float _12; float _13; float _14;
			float _21; float _22; float _23; float _24;
			float _31; float _32; float _33; float _34;
			float _41; float _42; float _43; float _44;
		};
	};

	Matrix4() = default;
	Matrix4(float a1, float a2, float a3, float a4,
		float b1, float b2, float b3, float b4,
		float c1, float c2, float c3, float c4,
		float d1, float d2, float d3, float d4)
	{
		_11 = a1; _12 = a2; _13 = a3; _14 = a4;
		_21 = b1; _22 = b2; _23 = b3; _24 = b4;
		_31 = c1; _32 = c2; _33 = c3; _34 = c4;
		_41 = d1; _42 = d2; _43 = d3; _44 = d4;
	}
	void SetZero()
	{
		m[0][0] = m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][1] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = 0.0f;
	}
	void Identity()
	{
		_11 = 1.f; _12 = 0.f; _13 = 0.f; _14 = 0.f;
		_21 = 0.f; _22 = 1.f; _23 = 0.f; _24 = 0.f;
		_31 = 0.f; _32 = 0.f; _33 = 1.f; _34 = 0.f;
		_41 = 0.f; _42 = 0.f; _43 = 0.f; _44 = 1.f;
	}
	/*
	Matrix4& operator=(const Matrix4& rhs) {
		if(rhs==this)
			return this;
		else{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] = rhs.m[i][j];
		return *this;
		}
	}*/
	Matrix4 operator*(const Matrix4& rhs) const {
		Matrix4 t;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				t.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j];
		return t;
	}
	Matrix4 transpose() const {
		Matrix4 t;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				t.m[i][j] = m[j][i];
		return t;
	}//end of transpose()
	
	//用于点、向量与矩阵相乘统一公式
	Vec4 MultVec(const Vec4& rhs)
	{
		return Vec4(
			rhs.x*_11 + rhs.y*_21 + rhs.z*_31+rhs.w*_41,
			rhs.x*_12 + rhs.y*_22 + rhs.z*_32+rhs.w*_42,
			rhs.x*_13+ rhs.y*_23 + rhs.z*_33+rhs.w*_43,
			rhs.x*_14 + rhs.y*_24 + rhs.z*_34+rhs.w*_44);	
	}
};// end struct
