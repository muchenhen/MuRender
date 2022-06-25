#include "MathSet.h"


float MathSet::lerp(float x1, float x2, float t)
{
	return (x1 + t * (x2 - x1));
}


MuVector MathSet::lerp(const MuVector& v1, const MuVector& v2, float t)
{
	return MuVector(
		(v1.X + (v2.X - v1.X) * t),
		(v1.Y + (v2.Y - v1.Y) * t),
		(v1.Z + (v2.Z - v1.Z) * t),
		1
	);
}

MuVector3D MathSet::lerp(const MuVector3D& v1, const MuVector3D& v2, float t)
{
	return MuVector3D(
		(v1.X + (v2.X - v1.X) * t),
		(v1.Y + (v2.Y - v1.Y) * t),
		(v1.Z + (v2.Z - v1.Z) * t)
	);
}

MuVector2D MathSet::lerp(const MuVector2D& v1, const MuVector2D& v2, float t)
{
	return MuVector2D(
		(v1.U + (v2.U - v1.U) * t),
		(v1.V + (v2.V - v1.V) * t)
	);
}



float MathSet::max3(const float& a, const float& b, const float& c)
{
	return std::max<>(a, std::max<>(b, c));
}

float MathSet::min3(const float& a, const float& b, const float& c)
{
	return std::min<>(a, std::min<>(b, c));
}

float MathSet::convertToRadians(float fDegrees)
{
	return fDegrees * (PI / 180.0f);
}

Matrix MathSet::unitMatrix()
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix MathSet::transposedMatrix(const Matrix& m)
{
	return Matrix(
		m.m11,m.m21,m.m31,m.m41,
		m.m12,m.m22,m.m32,m.m42,
		m.m13,m.m23,m.m33,m.m43,
		m.m14,m.m24,m.m34,m.m44
	);
}

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

float MathSet::det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
	return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) + a3 * (b1 * c2 - c1 * b2);
}

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

Matrix MathSet::scaleMatrix(float scaleX, float scaleY, float scaleZ)
{
	return Matrix(
		scaleX, 0.0f, 0.0f, 0.0f,
		0.0f, scaleY, 0.0f, 0.0f,
		0.0f, 0.0f, scaleZ, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f
	);
}

Matrix MathSet::translationMatrix(float transX, float transY, float transZ)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		transX, transY, transZ, 1
	);
}

Matrix MathSet::rotationX(float a)
{
	return Matrix(
		1.0f,  0.0f,   0.0f,   0.0f,
		0.0f,  cosf(a), sinf(a), 0.0f,
		0.0f, -sinf(a), cosf(a), 0.0f,
		0.0f,  0.0f,   0.0f,   1.0f
	);
}

Matrix MathSet::rotationY(float a)
{
	return Matrix(
		cosf(a), 0.0f, -sinf(a), 0.0f,
		0.0f,   1.0f, 0.0f,    0.0f,
		sinf(a), 0.0f, cosf(a),  0.0f,
		0.0f,   0.0f, 0.0f,    1.0f
	);
}

Matrix MathSet::rotationZ(float a)
{
	return Matrix(
		cosf(a),  sinf(a), 0.0f, 0.0f,
		-sinf(a), cosf(a), 0.0f, 0.0f,
		0.0f,    0.0f,   1.0f, 0.0f,
		0.0f,    0.0f,   0.0f, 1.0f
	);
}

Matrix MathSet::rotateV(const MuVector3D& v, float a)
{
	return Matrix(
		v.X * v.X * (1 - cosf(a)) + cosf(a), v.X * v.Y * (1 - cosf(a)) + v.Z * sinf(a), v.X * v.Z * (1 - cosf(a) - v.Y * sinf(a)), 0.0f,
		v.X * v.Y * (1 - cosf(a)) - v.Z * sinf(a), v.Y * v.Y * (1 - cosf(a)) + cosf(a), v.Y * v.Z * (1 - cosf(a)) + v.X * sinf(a), 0.0f,
		v.X * v.Z * (1 - cosf(a)) + v.Y * sinf(a), v.Y * v.Z * (1 - cosf(a)) - v.X * sinf(a), v.Z * v.Z * (a - cosf(a)) + cosf(a), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix MathSet::MatrixLookAtLh(const MuVector& eyePosizition, const MuVector& FocusPosition, const MuVector& UpDirection)
{
	MuVector zaxis = (FocusPosition - eyePosizition);
	zaxis.Normalize();
	MuVector xaxis = MultiplicationCross(UpDirection, zaxis);
	xaxis.Normalize();
	MuVector yaxis = MultiplicationCross(zaxis, xaxis);

	return Matrix(
		xaxis.X,yaxis.X,zaxis.X,0.0f,
		xaxis.Y,yaxis.Y,zaxis.Y,0.0f,
		xaxis.Z,yaxis.Z,zaxis.Z,0.0f,
		-(xaxis*eyePosizition),-(yaxis*eyePosizition),-(zaxis*eyePosizition),1.0f
	);
}

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

unsigned int MathSet::color2UINT(const MuVector& color)
{
	int r = int(255 * color.X);
	int g = int(255 * color.Y);
	int b = int(255 * color.Z);
	
	r = clamp(r, 0, 255);
	g = clamp(g, 0, 255);
	b = clamp(b, 0, 255);
	return unsigned int((r << 16) | (g << 8) | (b));
}

MuVector MathSet::multVector4(const MuVector& v, const Matrix& m)
{
	return MuVector(
		v.X * m.m11 + v.Y * m.m21 + v.Z * m.m31 + v.W * m.m41,
		v.X * m.m12 + v.Y * m.m22 + v.Z * m.m32 + v.W * m.m42,
		v.X * m.m13 + v.Y * m.m23 + v.Z * m.m33 + v.W * m.m43,
		v.X * m.m14 + v.Y * m.m24 + v.Z * m.m34 + v.W * m.m44
	);
}

float MathSet::edgeFunc(const MuVector& a, const MuVector& b, const MuVector& p)
{
	return ((b.X - a.X) * (p.Y - a.Y) - (b.Y - a.Y) * (p.X - a.X));
}