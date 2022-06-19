#pragma once
#include <math.h>

/////////////////////////////////////////////////////////
//
//��ά������
//
/////////////////////////////////////////////////////////
class Vector2
{
public:
	float u,v;

	//Ĭ�Ϲ��캯��
	Vector2();
	//���ƹ��캯��
	Vector2(const Vector2& a);
	//�������캯��
	Vector2(float u1, float v1);

	~Vector2();

	//���������
	//��ֵ���������
	Vector2& operator = (const Vector2& a);
	//==���������
	bool operator == (const Vector2& a);
	//!+���������
	bool operator != (const Vector2& a);

	//����-���������ȡ��
	Vector2 operator - ();

	//��Ԫ�ӷ�
	Vector2 operator + (const Vector2& a);

	//�����˷�
	Vector2 operator * (float a);
	//��������
	Vector2 operator / (float a);

	//�Է����������
	//-=
	Vector2& operator -= (const Vector2& a);
	//+=
	Vector2& operator += (const Vector2& a);
	//*= �����
	Vector2& operator *= (float a);
	///= �����
	Vector2& operator /= (float a);

	//����*��ʵ���������
	float operator * (const Vector2& a);

	//������׼��
	void normalize();

};

//�ǳ�Ա����

//��������ģ
float modulus(const Vector2& a);

/*
//���������Ĳ��
Vector2 multiplicationCross(const Vector2& a, const Vector2& b);
//ʵ�ֱ������
*/

Vector2 operator * (float k, const Vector2& v);
//���������ľ���
float distance(const Vector2& a, const Vector2& b);

//ȫ�ֱ���
const Vector2 ZeroVector2(0.0f, 0.0f);

/////////////////////////////////////////////////////////
//
//��ά������
//
/////////////////////////////////////////////////////////
class Vector3
{
public:
	float x, y, z;

	//Ĭ�Ϲ��캯��
	Vector3();
	//���ƹ��캯��
	Vector3(const Vector3& a);
	//�������캯��
	Vector3(float ax, float ay, float az);

	~Vector3();

	//���������
	//��ֵ���������
	Vector3& operator = (const Vector3& a);
	//==���������
	bool operator == (const Vector3& a);
	//!+���������
	bool operator != (const Vector3& a);

	//����-���������ȡ��
	Vector3 operator - ();

	//��Ԫ�ӷ�
	Vector3 operator + (const Vector3& a);

	//�����˷�
	Vector3 operator * (float a);
	//��������
	Vector3 operator / (float a);

	//�Է����������
	//-=
	Vector3& operator -= (const Vector3& a);
	//+=
	Vector3& operator += (const Vector3& a);
	//*= �����
	Vector3& operator *= (float a);
	///= �����
	Vector3& operator /= (float a);

	//����*��ʵ���������
	float operator * (const Vector3& a);

	//������׼��
	void normalize();

};

//�ǳ�Ա����

//��������ģ
float modulus(const Vector3& a);
//���������Ĳ��
Vector3 multiplicationCross(const Vector3& a, const Vector3& b);
//ʵ�ֱ������
Vector3 operator * (float k, const Vector3& v);
//���������ľ���
float distance(const Vector3& a, const Vector3& b);

//ȫ�ֱ���
const Vector3 ZeroVector3(0.0f, 0.0f, 0.0f);


/////////////////////////////////////////////////////////
//
//��ά������
//
/////////////////////////////////////////////////////////
class Vector4
{
public:
	float x, y, z, w;

	//Ĭ�Ϲ��캯��
	Vector4();
	//���ƹ��캯��
	Vector4(const Vector4& a);
	Vector4(const Vector3& a);
	//�������캯��
	Vector4(float ax, float ay, float az, float aw);
	Vector4(float ax, float ay, float az);
	Vector4(float u, float v);
	//��������
	~Vector4();

	//���������
	//��ֵ���������
	Vector4& operator = (const Vector4& a);
	//==���������
	bool operator == (const Vector4& a);
	//!+���������
	bool operator != (const Vector4& a);

	//����-���������ȡ��
	Vector4 operator - () const;

	//��Ԫ�ӷ�
	Vector4 operator + (const Vector4& a)const;

	//��Ԫ����
	Vector4 operator - (const Vector4& a) const;

	//�����˷�
	Vector4 operator * (float a) const;
	//��������
	Vector4 operator / (float a) const;

	//�Է����������
	//-=
	Vector4& operator -= (const Vector4& a);
	//+=
	Vector4& operator += (const Vector4& a);
	//*= �����
	Vector4& operator *= (float a);
	///= �����
	Vector4& operator /= (float a);

	//����*��ʵ���������
	float operator * (const Vector4& a)const;

	//������׼��
	void normalize();

};

//�ǳ�Ա����

//��������ģ
float modulus(const Vector4& a);
//���������Ĳ��
Vector4 multiplicationCross(const Vector4& a, const Vector4& b);
//ʵ�ֱ������
Vector4 operator * (float k, const Vector4& v);
//���������ľ���
float distance(const Vector4& a, const Vector4& b);

//ȫ�ֱ���
const Vector4 ZeroVector4(0.0f, 0.0f, 0.0f, 1.0f);