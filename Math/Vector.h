#pragma once


class MuVector2D
{
public:
	float U, V;

	MuVector2D();
	MuVector2D(float U1, float V1);
	MuVector2D(const MuVector2D& A);

	~MuVector2D();

	MuVector2D& operator = (const MuVector2D& A);
	bool operator == (const MuVector2D& A);
	bool operator != (const MuVector2D& A);
	MuVector2D operator - ();
	MuVector2D operator + (const MuVector2D& A);
	MuVector2D operator * (float A);
	MuVector2D operator / (float A);
	MuVector2D& operator -= (const MuVector2D& A);
	MuVector2D& operator += (const MuVector2D& A);
	MuVector2D& operator *= (float A);
	MuVector2D& operator /= (float A);
	float operator * (const MuVector2D& A);

	void Normalize();
};

float Modulus(const MuVector2D& A);

/*
MuVector2D multiplicationCross(const MuVector2D& a, const MuVector2D& b);
*/

MuVector2D operator * (float K, const MuVector2D& V);
float Distance(const MuVector2D& A, const MuVector2D& B);


class MuVector3D
{
public:
	float X, Y, Z;

	MuVector3D();
	MuVector3D(const MuVector3D& A);
	MuVector3D(float InX, float InY, float InZ);

	~MuVector3D();

	MuVector3D& operator = (const MuVector3D& A);
	bool operator == (const MuVector3D& A);
	bool operator != (const MuVector3D& A);
	MuVector3D operator - ();
	MuVector3D operator + (const MuVector3D& A);
	MuVector3D operator * (float A);
	MuVector3D operator / (float A);
	MuVector3D& operator -= (const MuVector3D& A);
	MuVector3D& operator += (const MuVector3D& A);
	MuVector3D& operator *= (float A);
	MuVector3D& operator /= (float A);
	float operator * (const MuVector3D& A);

	void Normalize();
};


float Modulus(const MuVector3D& A);
MuVector3D MultiplicationCross(const MuVector3D& A, const MuVector3D& B);
MuVector3D operator * (float K, const MuVector3D& V);
float Distance(const MuVector3D& A, const MuVector3D& B);


class MuVector
{
public:
	float X, Y, Z, W;

	MuVector();
	MuVector(const MuVector& a);
	MuVector(const MuVector3D& a);
	MuVector(float ax, float ay, float az, float aw);
	MuVector(float ax, float ay, float az);
	MuVector(float u, float v);
	~MuVector();

	MuVector& operator = (const MuVector& a);
	bool operator == (const MuVector& a);
	bool operator != (const MuVector& a);
	MuVector operator - () const;
	MuVector operator + (const MuVector& a)const;
	MuVector operator - (const MuVector& a) const;
	MuVector operator * (float a) const;
	MuVector operator / (float a) const;
	MuVector& operator -= (const MuVector& a);
	MuVector& operator += (const MuVector& a);
	MuVector& operator *= (float a);
	MuVector& operator /= (float a);

	float operator * (const MuVector& a)const;

	void Normalize();
};


float Modulus(const MuVector& a);
MuVector MultiplicationCross(const MuVector& a, const MuVector& b);
MuVector operator * (float k, const MuVector& v);
float Distance(const MuVector& a, const MuVector& b);

namespace MuVectors
{
	const MuVector2D ZeroVector2(0.0f, 0.0f);
	const MuVector3D ZeroVector3(0.0f, 0.0f, 0.0f);
	const MuVector ZeroVector4(0.0f, 0.0f, 0.0f, 1.0f);

}
