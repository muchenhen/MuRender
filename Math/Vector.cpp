#include "Vector.h"


/////////////////////
//
//��ά������
//
/////////////////////

//Ĭ�Ϲ��캯��
Vector2::Vector2()
{
	u = 0; v = 0;
}

//���ƹ��캯��
Vector2::Vector2(const Vector2& a)
{
	u = a.u;
	v = a.v;
}

//�������캯��
Vector2::Vector2(float u1, float v1)
{
	u = u1;
	v = v1;
}

//��������
Vector2::~Vector2()
{

}

//���������

//��ֵ
Vector2& Vector2::operator=(const Vector2& a)
{
	u = a.u;
	v = a.v;
	return *this;
}

//�ж��Ƿ����
bool Vector2::operator==(const Vector2& a)
{
	return (u == a.u && v == a.v);
}

//!=Ԫ�ط����� �ж�����Vector�Ƿ����
bool Vector2::operator!=(const Vector2& a)
{
	return (u != a.u || v != a.v);
}

//����-���������ȡ��
Vector2 Vector2::operator-()
{
	return Vector2(-u, -v);
}

//��Ԫ�ӷ�
Vector2 Vector2::operator+(const Vector2& a)
{
	return Vector2(u + a.u, v + a.v);
}

//�����˷� �����ҳ�
Vector2 Vector2::operator*(float a)
{
	return Vector2(a * u, a * v);
}

//��������
Vector2 Vector2::operator/(float a)
{
	a = 1.0f / a;
	return Vector2(a * u, a * v);
}

//�Է����������

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

// ����*��ʵ���������
float Vector2::operator*(const Vector2& a)
{
	return (u * a.u + v * a.v);
}

//������׼��
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

//�ǳ�Ա����

//��ģ
float modulus(const Vector2& a)
{
	return sqrtf(a.u * a.u + a.v * a.v);
}

/*//�����������
Vector2 multiplicationCross(const Vector2& a, const Vector2& b)
{
	return Vector2(a.u * b.v - a.v * b.u);
}*/

//�������
Vector2 operator*(float k, const Vector2& v)
{
	return Vector2(k * v.u, k * v.v);
}

//������������
float distance(const Vector2& a, const Vector2& b)
{
	float dx = a.u - b.u;
	float dy = a.v - b.v;
	return sqrtf(dx * dx + dy * dy);
}


/////////////////////
//
//��ά������
//
/////////////////////

//Ĭ�Ϲ��캯��
Vector3::Vector3()
{
	x = 0; y = 0; z = 0;
}

//���ƹ��캯��
Vector3::Vector3(const Vector3& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

//�������캯��
Vector3::Vector3(float ax, float ay, float az)
{
	x = ax;
	y = ay;
	z = az;
}

//��������
Vector3::~Vector3()
{

}

//���������

//��ֵ
Vector3& Vector3::operator=(const Vector3& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

//�ж��Ƿ����
bool Vector3::operator==(const Vector3& a)
{
	return (x == a.x && y == a.y && z == a.z);
}

//!=Ԫ�ط����� �ж�����Vector�Ƿ����
bool Vector3::operator!=(const Vector3& a)
{
	return (x != a.x || y != a.y || z != a.z);
}

//����-���������ȡ��
Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}

//��Ԫ�ӷ�
Vector3 Vector3::operator+(const Vector3& a)
{
	return Vector3(x + a.x, y + a.y, z + a.z);
}

//�����˷� �����ҳ�
Vector3 Vector3::operator*(float a)
{
	return Vector3(a * x, a * y, a * z);
}

//��������
Vector3 Vector3::operator/(float a)
{
	a = 1.0f / a;
	return Vector3(a * x, a * y, a * z);
}

//�Է����������

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

// ����*��ʵ���������
float Vector3::operator*(const Vector3& a)
{
	return (x * a.x + y * a.y + z * a.z);
}

//������׼��
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

//�ǳ�Ա����

//��ģ
float modulus(const Vector3& a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

//�����������
Vector3 multiplicationCross(const Vector3& a, const Vector3& b)
{
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

//�������
Vector3 operator*(float k, const Vector3& v)
{
	return Vector3(k * v.x, k * v.y, k * v.z);
}

//������������
float distance(const Vector3& a, const Vector3& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}


/////////////////////
//
//��ά������
//
/////////////////////

//Ĭ�Ϲ��캯��
Vector4::Vector4()
{
	x = y = z = 0; w = 1;
}

//���ƹ��캯��
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

//�������캯��
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

//��������
Vector4::~Vector4()
{

}

//���������

//��ֵ���������
Vector4& Vector4::operator=(const Vector4& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;
	return *this;
}

//==��������� �ж�����Vector�Ƿ����
bool Vector4::operator==(const Vector4& a)
{
	return (x == a.x && y == a.y && z == a.z && w == a.w);
}

//!=Ԫ�ط����� �ж�����Vector�Ƿ����
bool Vector4::operator!=(const Vector4& a)
{
	return (x != a.x || y != a.y || z != a.z || w != a.w);
}

//����-���������ȡ��
Vector4 Vector4::operator - () const
{
	return Vector4(-x, -y, -z, 1);
}

//��Ԫ�ӷ�
Vector4 Vector4::operator+(const Vector4& a) const
{
	return Vector4(x + a.x, y + a.y, z + a.z, 1);
}

//��Ԫ����
Vector4 Vector4::operator-(const Vector4& a) const
{
	return Vector4(x - a.x, y - a.y, z - a.z, 1);
}

//�����˷� �����ҳ�
Vector4 Vector4::operator*(float a) const
{
	return Vector4(a * x, a * y, a * z, 1);
}

//��������
Vector4 Vector4::operator/(float a) const
{
	float t = 1.0f / a;
	return Vector4(t * x, t * y, t * z, 1);
}

//�Է����������

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

// ����*��ʵ���������
float Vector4::operator*(const Vector4& a) const
{
	return (x * a.x + y * a.y + z * a.z);
}

//������׼��
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

//�ǳ�Ա����

//��ģ
float modulus(const Vector4& a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

//�����������
Vector4 multiplicationCross(const Vector4& a, const Vector4& b)
{
	return Vector4(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
		1);
}

//�������
Vector4 operator*(float k, const Vector4& v)
{
	return Vector4(k * v.x, k * v.y, k * v.z, 1);
}

//������������
float distance(const Vector4& a, const Vector4& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}
