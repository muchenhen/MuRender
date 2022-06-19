/////////////////////////////////////
//
//���������ֵ���� �Ծ���Ĳ��� ���õĳ���
//
/////////////////////////////////////
#pragma once
#include <algorithm>
#include <windows.h>
#include "Matrix.h"

//����PI
const float Pi = 3.14159265f;

namespace MathSet
{
	/////////////////
	//
	//��ֵ���� ����tȡֵ��Χ��[0,1]
	//
	/////////////////

	//���Բ�ֵ����
	float lerp(float x1, float x2, float t);

	//ʸ����ֵ����

	//Vector4��ֵ
	Vector4 lerp(const Vector4& v1, const Vector4& v2, float t);

	//Vector3��ֵ
	Vector3 lerp(const Vector3& v1, const Vector3& v2, float t);

	//Vector2��ֵ
	Vector2 lerp(const Vector2& v1, const Vector2& v2, float t);

	//�����������ֵ��м��� 
	template <typename T>
	T clamp(T x, T min, T max)
	{
		if (x <= min)
			return min;
		else if (x >= max)
			return max;
		return x;
	}

	//�����������ֵ����ֵ
	float max3(const float& a, const float& b, const float& c);

	//�����������ֵ���Сֵ
	float min3(const float& a, const float& b, const float& c);

	//�Ƕ�ת���ɻ���
	float convertToRadians(float fDegrees); 

	//����һ����λ����
	Matrix unitMatrix();

	//����һ�������ת��
	Matrix transposedMatrix(const Matrix& m);

	//������������ʽ
	float det(const Matrix& m);

	//����3x3����ʽ��ֵ
	/*
	���ڼ���4*4����Ĵ�������ʽ
	*/
	float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3);

	//�����׼������� 
	/*
	����������ʽ��ɵľ���ת��
	*/
	Matrix adjMatrix(const Matrix& m);

	//����������
	/*
	��׼������� / ��������ʽ
	*/
	Matrix inverseMatrix(const Matrix& m);

	//���ž���
	Matrix scaleMatrix(float scaleX, float scaleY, float scaleZ);

	//ƽ�ƾ���
	Matrix translationMatrix(float transX, float transY, float transZ);

	//��x����ת
	Matrix rotationX(float a);

	//��y����ת
	Matrix rotationY(float a);

	//��z����ת
	Matrix rotationZ(float a);

	//����������ת
	Matrix rotateV(const Vector3& v, float a);

	//��ȡ�ӽǣ������������
	Matrix MatrixLookAtLh(const Vector4& eyePosizition, const Vector4& FocusPosition, const Vector4& UpDirection);

	//��ȡͶӰ����
	Matrix MatrixPerspectiveFovLH(const float& FovAngleY, const float& AspectRatio, const float& NearZ, const float& FarZ);

	//��ɫת��
	unsigned int color2UINT(const Vector4& color);

	//������˾���
	Vector4 multVector4(const Vector4& v, const Matrix& m);

	//�����жϵ��Ƿ�����������
	float edgeFunc(const Vector4& a, const Vector4& b, const Vector4& p);
}