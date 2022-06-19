#include "Vector.h"


/////////////////////
//
//二维向量类
//
/////////////////////

//默认构造函数
Vector2::Vector2()
{
	u = 0; v = 0;
}

//复制构造函数
Vector2::Vector2(const Vector2& a)
{
	u = a.u;
	v = a.v;
}

//参数构造函数
Vector2::Vector2(float u1, float v1)
{
	u = u1;
	v = v1;
}

//析构函数
Vector2::~Vector2()
{

}

//运算符重载

//赋值
Vector2& Vector2::operator=(const Vector2& a)
{
	u = a.u;
	v = a.v;
	return *this;
}

//判断是否相等
bool Vector2::operator==(const Vector2& a)
{
	return (u == a.u && v == a.v);
}

//!=元素符重载 判断两个Vector是否不相等
bool Vector2::operator!=(const Vector2& a)
{
	return (u != a.u || v != a.v);
}

//重载-运算符用于取反
Vector2 Vector2::operator-()
{
	return Vector2(-u, -v);
}

//二元加法
Vector2 Vector2::operator+(const Vector2& a)
{
	return Vector2(u + a.u, v + a.v);
}

//标量乘法 标量右乘
Vector2 Vector2::operator*(float a)
{
	return Vector2(a * u, a * v);
}

//标量除法
Vector2 Vector2::operator/(float a)
{
	a = 1.0f / a;
	return Vector2(a * u, a * v);
}

//自反运算符重载

// -= 
Vector2& Vector2::operator-=(const Vector2& a)
{
	u -= a.u;
	v -= a.v;
	return *this;
}

// +=
Vector2& Vector2::operator+=(const Vector2& a)
{
	u += a.v;
	v += a.v;
	return *this;
}

// *=
Vector2& Vector2::operator*=(float a)
{
	u *= a;
	v *= a;
	return *this;
}

// /=
Vector2& Vector2::operator/=(float a)
{
	a = 1.0f / a;
	u *= a;
	v *= a;
	return *this;
}

// 重载*号实现向量点乘
float Vector2::operator*(const Vector2& a)
{
	return (u * a.u + v * a.v);
}

//向量标准化
void Vector2::normalize()
{
	float modulus = sqrtf(u * u + v * v);
	if (modulus > 0.0f)
	{
		modulus = 1.0f / modulus;
		u *= modulus;
		v *= modulus;
	}
}

//非成员函数

//求模
float modulus(const Vector2& a)
{
	return sqrtf(a.u * a.u + a.v * a.v);
}

/*//计算向量叉乘
Vector2 multiplicationCross(const Vector2& a, const Vector2& b)
{
	return Vector2(a.u * b.v - a.v * b.u);
}*/

//标量左乘
Vector2 operator*(float k, const Vector2& v)
{
	return Vector2(k * v.u, k * v.v);
}

//计算两点间距离
float distance(const Vector2& a, const Vector2& b)
{
	float dx = a.u - b.u;
	float dy = a.v - b.v;
	return sqrtf(dx * dx + dy * dy);
}


/////////////////////
//
//三维向量类
//
/////////////////////

//默认构造函数
Vector3::Vector3()
{
	x = 0; y = 0; z = 0;
}

//复制构造函数
Vector3::Vector3(const Vector3& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

//参数构造函数
Vector3::Vector3(float ax, float ay, float az)
{
	x = ax;
	y = ay;
	z = az;
}

//析构函数
Vector3::~Vector3()
{

}

//运算符重载

//赋值
Vector3& Vector3::operator=(const Vector3& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

//判断是否相等
bool Vector3::operator==(const Vector3& a)
{
	return (x == a.x && y == a.y && z == a.z);
}

//!=元素符重载 判断两个Vector是否不相等
bool Vector3::operator!=(const Vector3& a)
{
	return (x != a.x || y != a.y || z != a.z);
}

//重载-运算符用于取反
Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}

//二元加法
Vector3 Vector3::operator+(const Vector3& a)
{
	return Vector3(x + a.x, y + a.y, z + a.z);
}

//标量乘法 标量右乘
Vector3 Vector3::operator*(float a)
{
	return Vector3(a * x, a * y, a * z);
}

//标量除法
Vector3 Vector3::operator/(float a)
{
	a = 1.0f / a;
	return Vector3(a * x, a * y, a * z);
}

//自反运算符重载

// -= 
Vector3& Vector3::operator-=(const Vector3& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

// +=
Vector3& Vector3::operator+=(const Vector3& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

// *=
Vector3& Vector3::operator*=(float a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

// /=
Vector3& Vector3::operator/=(float a)
{
	a = 1.0f / a;
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

// 重载*号实现向量点乘
float Vector3::operator*(const Vector3& a)
{
	return (x * a.x + y * a.y + z * a.z);
}

//向量标准化
void Vector3::normalize()
{
	float modulus = sqrtf(x * x + y * y + z * z);
	if (modulus > 0.0f)
	{
		modulus = 1.0f / modulus;
		x *= modulus;
		y *= modulus;
		z *= modulus;
	}
}

//非成员函数

//求模
float modulus(const Vector3& a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

//计算向量叉乘
Vector3 multiplicationCross(const Vector3& a, const Vector3& b)
{
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

//标量左乘
Vector3 operator*(float k, const Vector3& v)
{
	return Vector3(k * v.x, k * v.y, k * v.z);
}

//计算两点间距离
float distance(const Vector3& a, const Vector3& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}


/////////////////////
//
//四维向量类
//
/////////////////////

//默认构造函数
Vector4::Vector4()
{
	x = y = z = 0; w = 1;
}

//复制构造函数
Vector4::Vector4(const Vector4& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;
}

Vector4::Vector4(const Vector3& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = 1.0f;
}

//参数构造函数
Vector4::Vector4(float ax, float ay, float az, float aw)
{
	x = ax;
	y = ay;
	z = az;
	w = aw;
}

Vector4::Vector4(float ax, float ay, float az)
{
	x = ax;
	y = ay;
	z = az;
	w = 1.0f;
}

Vector4::Vector4(float u, float v)
{
	x = u;
	y = v;
	z = 0.0f;
	w = 0.0f;
}

//析构函数
Vector4::~Vector4()
{

}

//运算符重载

//赋值运算符重载
Vector4& Vector4::operator=(const Vector4& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;
	return *this;
}

//==运算符重载 判断两个Vector是否相等
bool Vector4::operator==(const Vector4& a)
{
	return (x == a.x && y == a.y && z == a.z && w == a.w);
}

//!=元素符重载 判断两个Vector是否不相等
bool Vector4::operator!=(const Vector4& a)
{
	return (x != a.x || y != a.y || z != a.z || w != a.w);
}

//重载-运算符用于取反
Vector4 Vector4::operator - () const
{
	return Vector4(-x, -y, -z, 1);
}

//二元加法
Vector4 Vector4::operator+(const Vector4& a) const
{
	return Vector4(x + a.x, y + a.y, z + a.z, 1);
}

//二元减法
Vector4 Vector4::operator-(const Vector4& a) const
{
	return Vector4(x - a.x, y - a.y, z - a.z, 1);
}

//标量乘法 标量右乘
Vector4 Vector4::operator*(float a) const
{
	return Vector4(a * x, a * y, a * z, 1);
}

//标量除法
Vector4 Vector4::operator/(float a) const
{
	float t = 1.0f / a;
	return Vector4(t * x, t * y, t * z, 1);
}

//自反运算符重载

//-=
Vector4& Vector4::operator-=(const Vector4& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}


//+=
Vector4& Vector4::operator+=(const Vector4& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

//*=
Vector4& Vector4::operator*=(float a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

// /=
Vector4& Vector4::operator/=(float a)
{
	a = 1.0f / a;
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

// 重载*号实现向量点乘
float Vector4::operator*(const Vector4& a) const
{
	return (x * a.x + y * a.y + z * a.z);
}

//向量标准化
void Vector4::normalize()
{
	float modulus = sqrtf(x * x + y * y + z * z);
	if (modulus > 0.0f)
	{
		modulus = 1.0f / modulus;
		x *= modulus;
		y *= modulus;
		z *= modulus;
	}
}

//非成员函数

//求模
float modulus(const Vector4& a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

//计算向量叉乘
Vector4 multiplicationCross(const Vector4& a, const Vector4& b)
{
	return Vector4(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
		1);
}

//标量左乘
Vector4 operator*(float k, const Vector4& v)
{
	return Vector4(k * v.x, k * v.y, k * v.z, 1);
}

//计算两点间距离
float distance(const Vector4& a, const Vector4& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}
