#pragma once
#include "Vector.h"


class Matrix
{
public:
	
	union
	{
		float m[4][4];
		struct
		{
			float m11; float m12; float m13; float m14;
			float m21; float m22; float m23; float m24;
			float m31; float m32; float m33; float m34;
			float m41; float m42; float m43; float m44;
		};
	};

	//默认构造
	Matrix();
	//复制构造
	Matrix(const Matrix& a);
	//参数构造
	Matrix(float a11, float a12, float a13, float a14,
		float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34,
		float a41, float a42, float a43, float a44);
	//参数构造
	Matrix(const float a[4][4]);

	//置为0
	void setZeroMatrix();
	//置为单位阵
	void setUnitMatrix();

	//重载运算符
	//数乘
	Matrix& operator *(float x);
	//矩阵乘法
	Matrix operator *(const Matrix& m);
	//矩阵加法
	Matrix& operator +(const Matrix& m);
	//矩阵减法
	Matrix& operator -(const Matrix& m);
	//矩阵相等
	bool operator == (const Matrix& m);


	//成员函数转置矩阵
	Matrix& transpose();

};

//实现常数左乘
inline Matrix operator*(float k, const Matrix& m);

//转置矩阵
inline Matrix transpose(const Matrix& m);

