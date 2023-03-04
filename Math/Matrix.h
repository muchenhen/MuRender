#pragma once

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

	Matrix();
	Matrix(const Matrix& a);
	Matrix(float a11, float a12, float a13, float a14,
	float a21, float a22, float a23, float a24,
	float a31, float a32, float a33, float a34,
	float a41, float a42, float a43, float a44);
	Matrix(const float a[4][4]);

    ~Matrix();

	void setZeroMatrix();
	void setUnitMatrix();

	Matrix& operator *(float x);
	Matrix operator *(const Matrix& m);
	Matrix& operator +(const Matrix& m);
	Matrix& operator -(const Matrix& m);
	bool operator == (const Matrix& m);


	Matrix& transpose();

};

inline Matrix operator*(float k, const Matrix& m);

inline Matrix transpose(const Matrix& m);

