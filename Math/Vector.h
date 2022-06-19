#pragma once
#include <math.h>

/////////////////////////////////////////////////////////
//
//二维向量类
//
/////////////////////////////////////////////////////////
class Vector2
{
public:
	float u,v;

	//默认构造函数
	Vector2();
	//复制构造函数
	Vector2(const Vector2& a);
	//参数构造函数
	Vector2(float u1, float v1);

	~Vector2();

	//运算符重载
	//赋值运算符重载
	Vector2& operator = (const Vector2& a);
	//==运算符重载
	bool operator == (const Vector2& a);
	//!+运算符重载
	bool operator != (const Vector2& a);

	//重载-运算符用于取反
	Vector2 operator - ();

	//二元加法
	Vector2 operator + (const Vector2& a);

	//标量乘法
	Vector2 operator * (float a);
	//标量除法
	Vector2 operator / (float a);

	//自反运算符重载
	//-=
	Vector2& operator -= (const Vector2& a);
	//+=
	Vector2& operator += (const Vector2& a);
	//*= 与标量
	Vector2& operator *= (float a);
	///= 与标量
	Vector2& operator /= (float a);

	//重载*号实现向量点乘
	float operator * (const Vector2& a);

	//向量标准化
	void normalize();

};

//非成员函数

//求向量的模
float modulus(const Vector2& a);

/*
//计算向量的叉乘
Vector2 multiplicationCross(const Vector2& a, const Vector2& b);
//实现标量左乘
*/

Vector2 operator * (float k, const Vector2& v);
//计算两点间的距离
float distance(const Vector2& a, const Vector2& b);

//全局变量
const Vector2 ZeroVector2(0.0f, 0.0f);

/////////////////////////////////////////////////////////
//
//三维向量类
//
/////////////////////////////////////////////////////////
class Vector3
{
public:
	float x, y, z;

	//默认构造函数
	Vector3();
	//复制构造函数
	Vector3(const Vector3& a);
	//参数构造函数
	Vector3(float ax, float ay, float az);

	~Vector3();

	//运算符重载
	//赋值运算符重载
	Vector3& operator = (const Vector3& a);
	//==运算符重载
	bool operator == (const Vector3& a);
	//!+运算符重载
	bool operator != (const Vector3& a);

	//重载-运算符用于取反
	Vector3 operator - ();

	//二元加法
	Vector3 operator + (const Vector3& a);

	//标量乘法
	Vector3 operator * (float a);
	//标量除法
	Vector3 operator / (float a);

	//自反运算符重载
	//-=
	Vector3& operator -= (const Vector3& a);
	//+=
	Vector3& operator += (const Vector3& a);
	//*= 与标量
	Vector3& operator *= (float a);
	///= 与标量
	Vector3& operator /= (float a);

	//重载*号实现向量点乘
	float operator * (const Vector3& a);

	//向量标准化
	void normalize();

};

//非成员函数

//求向量的模
float modulus(const Vector3& a);
//计算向量的叉乘
Vector3 multiplicationCross(const Vector3& a, const Vector3& b);
//实现标量左乘
Vector3 operator * (float k, const Vector3& v);
//计算两点间的距离
float distance(const Vector3& a, const Vector3& b);

//全局变量
const Vector3 ZeroVector3(0.0f, 0.0f, 0.0f);


/////////////////////////////////////////////////////////
//
//四维向量类
//
/////////////////////////////////////////////////////////
class Vector4
{
public:
	float x, y, z, w;

	//默认构造函数
	Vector4();
	//复制构造函数
	Vector4(const Vector4& a);
	Vector4(const Vector3& a);
	//参数构造函数
	Vector4(float ax, float ay, float az, float aw);
	Vector4(float ax, float ay, float az);
	Vector4(float u, float v);
	//析构函数
	~Vector4();

	//运算符重载
	//赋值运算符重载
	Vector4& operator = (const Vector4& a);
	//==运算符重载
	bool operator == (const Vector4& a);
	//!+运算符重载
	bool operator != (const Vector4& a);

	//重载-运算符用于取反
	Vector4 operator - () const;

	//二元加法
	Vector4 operator + (const Vector4& a)const;

	//二元减法
	Vector4 operator - (const Vector4& a) const;

	//标量乘法
	Vector4 operator * (float a) const;
	//标量除法
	Vector4 operator / (float a) const;

	//自反运算符重载
	//-=
	Vector4& operator -= (const Vector4& a);
	//+=
	Vector4& operator += (const Vector4& a);
	//*= 与标量
	Vector4& operator *= (float a);
	///= 与标量
	Vector4& operator /= (float a);

	//重载*号实现向量点乘
	float operator * (const Vector4& a)const;

	//向量标准化
	void normalize();

};

//非成员函数

//求向量的模
float modulus(const Vector4& a);
//计算向量的叉乘
Vector4 multiplicationCross(const Vector4& a, const Vector4& b);
//实现标量左乘
Vector4 operator * (float k, const Vector4& v);
//计算两点间的距离
float distance(const Vector4& a, const Vector4& b);

//全局变量
const Vector4 ZeroVector4(0.0f, 0.0f, 0.0f, 1.0f);