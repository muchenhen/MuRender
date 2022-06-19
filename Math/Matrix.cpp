#include "Matrix.h"

Matrix::Matrix()
{
	
}

Matrix::Matrix(const Matrix& a)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = a.m[i][j];
		}
	}
}

Matrix::Matrix(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44)
{
	m11 = a11; m12 = a12; m13 = a13; m14 = a14;
	m21 = a21; m22 = a22; m23 = a23; m24 = a24;
	m31 = a31; m32 = a32; m33 = a33; m34 = a34;
	m41 = a41; m42 = a42; m43 = a43; m44 = a44;
}

Matrix::Matrix(const float a[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; j++)
		{
			m[i][j] = a[i][j];
		}
	}
}

void Matrix::setZeroMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = 0.0f;
		}
	}
}

void Matrix::setUnitMatrix()
{
	m11 = 1.f; m12 = 0.f; m13 = 0.f; m14 = 0.f;
	m21 = 0.f; m22 = 1.f; m23 = 0.f; m24 = 0.f;
	m31 = 0.f; m32 = 0.f; m33 = 1.f; m34 = 0.f;
	m41 = 0.f; m42 = 0.f; m43 = 0.f; m44 = 1.f;
}

Matrix& Matrix::operator*(float x)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; j++)
		{
			m[i][j] *= x;
		}
	}
	return *this;
}

Matrix Matrix::operator*(const Matrix& m)
{
	Matrix t;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			t.m[i][j] = this->m[i][0] * m.m[0][j] + this->m[i][1] * m.m[1][j] + this->m[i][2] * m.m[2][j] + this->m[i][3] * m.m[3][j];
	return t;
}

Matrix& Matrix::operator+(const Matrix& m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->m[i][j] += m.m[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-(const Matrix& m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->m[i][j] -= m.m[i][j];
		}
	}
	return *this;
}

bool Matrix::operator==(const Matrix& m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (this->m[i][j] != m.m[i][j])
			{
				return false;
			}			
		}
	}
	return true;
}

Matrix& Matrix::transpose()
{
	Matrix t(*this);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = t.m[j][i];
	return *this;
}

inline Matrix operator*(float k, const Matrix& m)
{
	Matrix t(m);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; j++)
		{
			t.m[i][j] = k;
		}
	}
	return t;
}

inline Matrix transpose(const Matrix& m)
{
	Matrix t;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			t.m[i][j] = m.m[j][i];
	return t;
}
