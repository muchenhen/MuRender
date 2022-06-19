/////////////////////////////////////
//
//包含各类插值函数 对矩阵的操作 常用的常量
//
/////////////////////////////////////
#pragma once
#include <algorithm>
#include <windows.h>
#include "Matrix.h"

//定义PI
const float Pi = 3.14159265f;

namespace MathSet
{
	/////////////////
	//
	//插值函数 参数t取值范围是[0,1]
	//
	/////////////////

	//线性插值函数
	float lerp(float x1, float x2, float t);

	//矢量插值函数

	//Vector4插值
	Vector4 lerp(const Vector4& v1, const Vector4& v2, float t);

	//Vector3插值
	Vector3 lerp(const Vector3& v1, const Vector3& v2, float t);

	//Vector2插值
	Vector2 lerp(const Vector2& v1, const Vector2& v2, float t);

	//返回三个数字的中间量 
	template <typename T>
	T clamp(T x, T min, T max)
	{
		if (x <= min)
			return min;
		else if (x >= max)
			return max;
		return x;
	}

	//返回三个数字的最大值
	float max3(const float& a, const float& b, const float& c);

	//返回三个数字的最小值
	float min3(const float& a, const float& b, const float& c);

	//角度转换成弧度
	float convertToRadians(float fDegrees); 

	//返回一个单位矩阵
	Matrix unitMatrix();

	//返回一个矩阵的转置
	Matrix transposedMatrix(const Matrix& m);

	//计算矩阵的行列式
	float det(const Matrix& m);

	//计算3x3行列式的值
	/*
	用于计算4*4方阵的代数余子式
	*/
	float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3);

	//计算标准伴随矩阵 
	/*
	将代数余子式组成的矩阵转置
	*/
	Matrix adjMatrix(const Matrix& m);

	//计算矩阵的逆
	/*
	标准伴随矩阵 / 矩阵行列式
	*/
	Matrix inverseMatrix(const Matrix& m);

	//缩放矩阵
	Matrix scaleMatrix(float scaleX, float scaleY, float scaleZ);

	//平移矩阵
	Matrix translationMatrix(float transX, float transY, float transZ);

	//绕x轴旋转
	Matrix rotationX(float a);

	//绕y轴旋转
	Matrix rotationY(float a);

	//绕z轴旋转
	Matrix rotationZ(float a);

	//绕任意轴旋转
	Matrix rotateV(const Vector3& v, float a);

	//获取视角（摄像机）矩阵
	Matrix MatrixLookAtLh(const Vector4& eyePosizition, const Vector4& FocusPosition, const Vector4& UpDirection);

	//获取投影矩阵
	Matrix MatrixPerspectiveFovLH(const float& FovAngleY, const float& AspectRatio, const float& NearZ, const float& FarZ);

	//颜色转换
	unsigned int color2UINT(const Vector4& color);

	//向量左乘矩阵
	Vector4 multVector4(const Vector4& v, const Matrix& m);

	//用于判断点是否在三角形内
	float edgeFunc(const Vector4& a, const Vector4& b, const Vector4& p);
}