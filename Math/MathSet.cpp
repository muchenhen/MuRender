#include "MathSet.h"

/////////////////
//
//插值函数 参数t取值范围是[0,1]
//
/////////////////

//线性插值函数
float MathSet::lerp(float x1, float x2, float t)
{
	return (x1 + t * (x2 - x1));
}

//矢量插值函数

//Vector4插值
Vector4 MathSet::lerp(const Vector4& v1, const Vector4& v2, float t)
{
	return Vector4(
		(v1.x + (v2.x - v1.x) * t),
		(v1.y + (v2.y - v1.y) * t),
		(v1.z + (v2.z - v1.z) * t),
		1
	);
}

//Vector3插值
Vector3 MathSet::lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return Vector3(
		(v1.x + (v2.x - v1.x) * t),
		(v1.y + (v2.y - v1.y) * t),
		(v1.z + (v2.z - v1.z) * t)
	);
}

//Vector2插值
Vector2 MathSet::lerp(const Vector2& v1, const Vector2& v2, float t)
{
	return Vector2(
		(v1.u + (v2.u - v1.u) * t),
		(v1.v + (v2.v - v1.v) * t)
	);
}



//返回三个数字的最大值
float MathSet::max3(const float& a, const float& b, const float& c)
{
	return std::max<>(a, std::max<>(b, c));
}

//返回三个数字的最小值
float MathSet::min3(const float& a, const float& b, const float& c)
{
	return std::min<>(a, std::min<>(b, c));
}

//角度转换成弧度
float MathSet::convertToRadians(float fDegrees)
{
	return fDegrees * (Pi / 180.0f);
}

//返回一个单位矩阵
Matrix MathSet::unitMatrix()
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//返回一个矩阵的转置
Matrix MathSet::transposedMatrix(const Matrix& m)
{
	return Matrix(
		m.m11,m.m21,m.m31,m.m41,
		m.m12,m.m22,m.m32,m.m42,
		m.m13,m.m23,m.m33,m.m43,
		m.m14,m.m24,m.m34,m.m44
	);
}

//计算矩阵的行列式
/*
直接利用公式进行计算
*/
float MathSet::det(const Matrix& m)
{
	float det =
		m.m11 * m.m22 * m.m33 * m.m44 - m.m11 * m.m22 * m.m34 * m.m43 -
		m.m11 * m.m23 * m.m32 * m.m44 + m.m11 * m.m23 * m.m34 * m.m42 +
		m.m11 * m.m24 * m.m32 * m.m43 - m.m11 * m.m24 * m.m33 * m.m42 -
		m.m12 * m.m21 * m.m33 * m.m44 + m.m12 * m.m21 * m.m34 * m.m43 +
		m.m12 * m.m23 * m.m31 * m.m44 - m.m12 * m.m23 * m.m34 * m.m41 -
		m.m12 * m.m24 * m.m31 * m.m43 + m.m12 * m.m24 * m.m33 * m.m41 +
		m.m13 * m.m21 * m.m32 * m.m44 - m.m13 * m.m21 * m.m34 * m.m42 -
		m.m13 * m.m22 * m.m31 * m.m44 + m.m13 * m.m22 * m.m34 * m.m41 +
		m.m13 * m.m24 * m.m31 * m.m42 - m.m13 * m.m24 * m.m32 * m.m41 -
		m.m14 * m.m21 * m.m32 * m.m43 + m.m14 * m.m21 * m.m33 * m.m42 +
		m.m14 * m.m22 * m.m31 * m.m43 - m.m14 * m.m22 * m.m33 * m.m41 -
		m.m14 * m.m23 * m.m31 * m.m42 + m.m14 * m.m23 * m.m32 * m.m41 ;
	return det;
}

//计算3x3行列式的值
/*
用于计算4*4方阵的代数余子式
*/
float MathSet::det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
	return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) + a3 * (b1 * c2 - c1 * b2);
}

//计算标准伴随矩阵 
/*
将代数余子式组成的矩阵转置
*/
Matrix MathSet::adjMatrix(const Matrix& m)
{
	float a1 = det3x3(m.m22, m.m23, m.m24, m.m32, m.m33, m.m34, m.m42, m.m43, m.m44);
	float a2 = det3x3(m.m21, m.m23, m.m24, m.m31, m.m33, m.m34, m.m41, m.m43, m.m44);
	float a3 = det3x3(m.m21, m.m22, m.m24, m.m31, m.m32, m.m34, m.m41, m.m42, m.m44);
	float a4 = det3x3(m.m21, m.m22, m.m23, m.m31, m.m32, m.m33, m.m41, m.m42, m.m43);
	float b1 = det3x3(m.m12, m.m13, m.m14, m.m32, m.m33, m.m34, m.m42, m.m43, m.m44);
	float b2 = det3x3(m.m11, m.m13, m.m14, m.m31, m.m33, m.m34, m.m41, m.m43, m.m44);
	float b3 = det3x3(m.m11, m.m12, m.m14, m.m31, m.m32, m.m34, m.m41, m.m42, m.m44);
	float b4 = det3x3(m.m11, m.m12, m.m13, m.m31, m.m32, m.m33, m.m41, m.m42, m.m43);
	float c1 = det3x3(m.m12, m.m13, m.m14, m.m22, m.m23, m.m24, m.m42, m.m43, m.m44);
	float c2 = det3x3(m.m11, m.m13, m.m14, m.m21, m.m23, m.m24, m.m41, m.m43, m.m44);
	float c3 = det3x3(m.m11, m.m12, m.m14, m.m21, m.m22, m.m24, m.m41, m.m42, m.m44);
	float c4 = det3x3(m.m11, m.m12, m.m13, m.m21, m.m22, m.m23, m.m41, m.m42, m.m43);
	float d1 = det3x3(m.m12, m.m13, m.m14, m.m22, m.m23, m.m24, m.m32, m.m33, m.m34);
	float d2 = det3x3(m.m11, m.m13, m.m14, m.m21, m.m23, m.m24, m.m31, m.m33, m.m34);
	float d3 = det3x3(m.m11, m.m12, m.m14, m.m21, m.m22, m.m24, m.m31, m.m32, m.m34);
	float d4 = det3x3(m.m11, m.m12, m.m13, m.m21, m.m22, m.m23, m.m31, m.m32, m.m33);

	return transposedMatrix(Matrix(
		 a1, -a2,  a3, -a4,
		-b1,  b2, -b3,  b4,
		 c1, -c2,  c3, -c4,
		-d1,  d2, -d3,  d4
	));
}

//计算矩阵的逆
/*
标准伴随矩阵 / 矩阵行列式
*/
Matrix MathSet::inverseMatrix(const Matrix& m)
{
	float Det = abs(det(m));
	Det = 1.0f / Det;
	Matrix adjM = adjMatrix(m);
	Matrix inverseM;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverseM.m[i][j] = adjM.m[i][j] * Det;
		}
	}
	return inverseM;
}

//缩放矩阵
Matrix MathSet::scaleMatrix(float scaleX, float scaleY, float scaleZ)
{
	return Matrix(
		scaleX, 0.0f, 0.0f, 0.0f,
		0.0f, scaleY, 0.0f, 0.0f,
		0.0f, 0.0f, scaleZ, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f
	);
}

//平移矩阵
Matrix MathSet::translationMatrix(float transX, float transY, float transZ)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		transX, transY, transZ, 1
	);
}

//绕X轴旋转
Matrix MathSet::rotationX(float a)
{
	return Matrix(
		1.0f,  0.0f,   0.0f,   0.0f,
		0.0f,  cosf(a), sinf(a), 0.0f,
		0.0f, -sinf(a), cosf(a), 0.0f,
		0.0f,  0.0f,   0.0f,   1.0f
	);
}

//绕Y轴旋转
Matrix MathSet::rotationY(float a)
{
	return Matrix(
		cosf(a), 0.0f, -sinf(a), 0.0f,
		0.0f,   1.0f, 0.0f,    0.0f,
		sinf(a), 0.0f, cosf(a),  0.0f,
		0.0f,   0.0f, 0.0f,    1.0f
	);
}

//绕Z轴旋转
Matrix MathSet::rotationZ(float a)
{
	return Matrix(
		cosf(a),  sinf(a), 0.0f, 0.0f,
		-sinf(a), cosf(a), 0.0f, 0.0f,
		0.0f,    0.0f,   1.0f, 0.0f,
		0.0f,    0.0f,   0.0f, 1.0f
	);
}

//绕任意轴旋转
Matrix MathSet::rotateV(const Vector3& v, float a)
{
	return Matrix(
		v.x * v.x * (1 - cosf(a)) + cosf(a), v.x * v.y * (1 - cosf(a)) + v.z * sinf(a), v.x * v.z * (1 - cosf(a) - v.y * sinf(a)), 0.0f,
		v.x * v.y * (1 - cosf(a)) - v.z * sinf(a), v.y * v.y * (1 - cosf(a)) + cosf(a), v.y * v.z * (1 - cosf(a)) + v.x * sinf(a), 0.0f,
		v.x * v.z * (1 - cosf(a)) + v.y * sinf(a), v.y * v.z * (1 - cosf(a)) - v.x * sinf(a), v.z * v.z * (a - cosf(a)) + cosf(a), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//获取视角（摄像机）矩阵
/*
 Vector4 EyePosition, 摄像机位置
 Vector4 FocusPosition, 焦点位置
 Vector4 UpDirection​​，摄像机的竖直方向，一般是0，1，0，0
*/
Matrix MathSet::MatrixLookAtLh(const Vector4& eyePosizition, const Vector4& FocusPosition, const Vector4& UpDirection)
{
	Vector4 zaxis = (FocusPosition - eyePosizition);
	zaxis.normalize();
	Vector4 xaxis = multiplicationCross(UpDirection, zaxis);
	xaxis.normalize();
	Vector4 yaxis = multiplicationCross(zaxis, xaxis);

	return Matrix(
		xaxis.x,yaxis.x,zaxis.x,0.0f,
		xaxis.y,yaxis.y,zaxis.y,0.0f,
		xaxis.z,yaxis.z,zaxis.z,0.0f,
		-(xaxis*eyePosizition),-(yaxis*eyePosizition),-(zaxis*eyePosizition),1.0f
	);
}

//获取投影矩阵
/*
使用透视投影
​MatrixPerspectiveFovLH
返回投影矩阵​
​四个参数为
 float FovAngleY, 从上往下看的视锥体的角度
 float AspectRatio, 屏幕宽/高
 float NearZ, 与近平面的距离
​ float FarZ​，与远平面的距离​
*/
Matrix MathSet::MatrixPerspectiveFovLH(const float& FovAngleY, const float& AspectRatio, const float& NearZ, const float& FarZ)
{
	Matrix mat;
	mat.setZeroMatrix();
	float fac = 1.0f / tanf(FovAngleY * 0.5f);
	mat.m11 = fac / AspectRatio;
	mat.m22 = fac;
	mat.m33 = FarZ / (FarZ - NearZ);
	mat.m34 = 1.0f;
	mat.m43 = -NearZ * FarZ / (FarZ - NearZ);
	return mat;
}

//颜色转换
unsigned int MathSet::color2UINT(const Vector4& color)
{
	int r = int(255 * color.x);
	int g = int(255 * color.y);
	int b = int(255 * color.z);
	
	r = clamp(r, 0, 255);
	g = clamp(g, 0, 255);
	b = clamp(b, 0, 255);
	return unsigned int((r << 16) | (g << 8) | (b));
}

//向量左乘矩阵
Vector4 MathSet::multVector4(const Vector4& v, const Matrix& m)
{
	return Vector4(
		v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41,
		v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42,
		v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43,
		v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44
	);
}

float MathSet::edgeFunc(const Vector4& a, const Vector4& b, const Vector4& p)
{
	return ((b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x));
}