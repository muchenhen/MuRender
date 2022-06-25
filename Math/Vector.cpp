#include "Vector.h"
#include <corecrt_math.h>

MuVector2D::MuVector2D()
{
	U = 0; V = 0;
}

MuVector2D::MuVector2D(const MuVector2D& A)
{
	U = A.U;
	V = A.V;
}

MuVector2D::MuVector2D(float U1, float V1)
{
	U = U1;
	V = V1;
}

MuVector2D::~MuVector2D()
= default;

MuVector2D& MuVector2D::operator=(const MuVector2D& A)
{
	U = A.U;
	V = A.V;
	return *this;
}

bool MuVector2D::operator==(const MuVector2D& A)
{
	return (U == A.U && V == A.V);
}

bool MuVector2D::operator!=(const MuVector2D& A)
{
	return (U != A.U || V != A.V);
}

MuVector2D MuVector2D::operator-()
{
	return MuVector2D(-U, -V);
}

MuVector2D MuVector2D::operator+(const MuVector2D& A)
{
	return MuVector2D(U + A.U, V + A.V);
}

MuVector2D MuVector2D::operator*(float A)
{
	return MuVector2D(A * U, A * V);
}

MuVector2D MuVector2D::operator/(float A)
{
	A = 1.0f / A;
	return MuVector2D(A * U, A * V);
}

MuVector2D& MuVector2D::operator-=(const MuVector2D& A)
{
	U -= A.U;
	V -= A.V;
	return *this;
}

MuVector2D& MuVector2D::operator+=(const MuVector2D& A)
{
	U += A.V;
	V += A.V;
	return *this;
}

MuVector2D& MuVector2D::operator*=(float A)
{
	U *= A;
	V *= A;
	return *this;
}

MuVector2D& MuVector2D::operator/=(float A)
{
	A = 1.0f / A;
	U *= A;
	V *= A;
	return *this;
}

float MuVector2D::operator*(const MuVector2D& A)
{
	return (U * A.U + V * A.V);
}

void MuVector2D::Normalize()
{
	float modulus = sqrtf(U * U + V * V);
	if (modulus > 0.0f)
	{
		modulus = 1.0f / modulus;
		U *= modulus;
		V *= modulus;
	}
}

float Modulus(const MuVector2D& A)
{
	return sqrtf(A.U * A.U + A.V * A.V);
}

MuVector2D operator*(float K, const MuVector2D& V)
{
	return MuVector2D(K * V.U, K * V.V);
}

float Distance(const MuVector2D& A, const MuVector2D& B)
{
	float dx = A.U - B.U;
	float dy = A.V - B.V;
	return sqrtf(dx * dx + dy * dy);
}


MuVector3D::MuVector3D()
{
	X = 0; Y = 0; Z = 0;
}

MuVector3D::MuVector3D(const MuVector3D& A)
{
	X = A.X;
	Y = A.Y;
	Z = A.Z;
}

MuVector3D::MuVector3D(float InX, float InY, float InZ)
{
	X = InX;
	Y = InY;
	Z = InZ;
}

MuVector3D::~MuVector3D()
{

}

MuVector3D& MuVector3D::operator=(const MuVector3D& A)
{
	X = A.X;
	Y = A.Y;
	Z = A.Z;
	return *this;
}

bool MuVector3D::operator==(const MuVector3D& A)
{
	return (X == A.X && Y == A.Y && Z == A.Z);
}

bool MuVector3D::operator!=(const MuVector3D& A)
{
	return (X != A.X || Y != A.Y || Z != A.Z);
}

MuVector3D MuVector3D::operator-()
{
	return MuVector3D(-X, -Y, -Z);
}

MuVector3D MuVector3D::operator+(const MuVector3D& A)
{
	return MuVector3D(X + A.X, Y + A.Y, Z + A.Z);
}

MuVector3D MuVector3D::operator*(float A)
{
	return MuVector3D(A * X, A * Y, A * Z);
}

MuVector3D MuVector3D::operator/(float A)
{
	A = 1.0f / A;
	return MuVector3D(A * X, A * Y, A * Z);
}


MuVector3D& MuVector3D::operator-=(const MuVector3D& A)
{
	X -= A.X;
	Y -= A.Y;
	Z -= A.Z;
	return *this;
}

MuVector3D& MuVector3D::operator+=(const MuVector3D& A)
{
	X += A.X;
	Y += A.Y;
	Z += A.Z;
	return *this;
}

MuVector3D& MuVector3D::operator*=(float A)
{
	X *= A;
	Y *= A;
	Z *= A;
	return *this;
}

MuVector3D& MuVector3D::operator/=(float A)
{
	A = 1.0f / A;
	X *= A;
	Y *= A;
	Z *= A;
	return *this;
}

float MuVector3D::operator*(const MuVector3D& A)
{
	return (X * A.X + Y * A.Y + Z * A.Z);
}

void MuVector3D::Normalize()
{
	float modulus = sqrtf(X * X + Y * Y + Z * Z);
	if (modulus > 0.0f)
	{
		modulus = 1.0f / modulus;
		X *= modulus;
		Y *= modulus;
		Z *= modulus;
	}
}


float Modulus(const MuVector3D& A)
{
	return sqrtf(A.X * A.X + A.Y * A.Y + A.Z * A.Z);
}

MuVector3D MultiplicationCross(const MuVector3D& A, const MuVector3D& B)
{
	return MuVector3D(
		A.Y * B.Z - A.Z * B.Y,
		A.Z * B.X - A.X * B.Z,
		A.X * B.Y - A.Y * B.X);
}

MuVector3D operator*(float K, const MuVector3D& V)
{
	return MuVector3D(K * V.X, K * V.Y, K * V.Z);
}

float Distance(const MuVector3D& A, const MuVector3D& B)
{
	float dx = A.X - B.X;
	float dy = A.Y - B.Y;
	float dz = A.Z - B.Z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}



MuVector::MuVector()
{
	X = Y = Z = 0; W = 1;
}

MuVector::MuVector(const MuVector& a)
{
	X = a.X;
	Y = a.Y;
	Z = a.Z;
	W = a.W;
}

MuVector::MuVector(const MuVector3D& a)
{
	X = a.X;
	Y = a.Y;
	Z = a.Z;
	W = 1.0f;
}

MuVector::MuVector(float ax, float ay, float az, float aw)
{
	X = ax;
	Y = ay;
	Z = az;
	W = aw;
}

MuVector::MuVector(float ax, float ay, float az)
{
	X = ax;
	Y = ay;
	Z = az;
	W = 1.0f;
}

MuVector::MuVector(float u, float v)
{
	X = u;
	Y = v;
	Z = 0.0f;
	W = 0.0f;
}

MuVector::~MuVector()
{

}


MuVector& MuVector::operator=(const MuVector& a)
{
	X = a.X;
	Y = a.Y;
	Z = a.Z;
	W = a.W;
	return *this;
}

bool MuVector::operator==(const MuVector& a)
{
	return (X == a.X && Y == a.Y && Z == a.Z && W == a.W);
}

bool MuVector::operator!=(const MuVector& a)
{
	return (X != a.X || Y != a.Y || Z != a.Z || W != a.W);
}

MuVector MuVector::operator - () const
{
	return MuVector(-X, -Y, -Z, 1);
}

MuVector MuVector::operator+(const MuVector& a) const
{
	return MuVector(X + a.X, Y + a.Y, Z + a.Z, 1);
}

MuVector MuVector::operator-(const MuVector& a) const
{
	return MuVector(X - a.X, Y - a.Y, Z - a.Z, 1);
}

MuVector MuVector::operator*(float a) const
{
	return MuVector(a * X, a * Y, a * Z, 1);
}

MuVector MuVector::operator/(float a) const
{
	float t = 1.0f / a;
	return MuVector(t * X, t * Y, t * Z, 1);
}


MuVector& MuVector::operator-=(const MuVector& a)
{
	X -= a.X;
	Y -= a.Y;
	Z -= a.Z;
	return *this;
}


MuVector& MuVector::operator+=(const MuVector& a)
{
	X += a.X;
	Y += a.Y;
	Z += a.Z;
	return *this;
}

MuVector& MuVector::operator*=(float a)
{
	X *= a;
	Y *= a;
	Z *= a;
	return *this;
}

MuVector& MuVector::operator/=(float a)
{
	a = 1.0f / a;
	X *= a;
	Y *= a;
	Z *= a;
	return *this;
}

float MuVector::operator*(const MuVector& a) const
{
	return (X * a.X + Y * a.Y + Z * a.Z);
}

void MuVector::Normalize()
{
	float modulus = sqrtf(X * X + Y * Y + Z * Z);
	if (modulus > 0.0f)
	{
		modulus = 1.0f / modulus;
		X *= modulus;
		Y *= modulus;
		Z *= modulus;
	}
}


float Modulus(const MuVector& a)
{
	return sqrtf(a.X * a.X + a.Y * a.Y + a.Z * a.Z);
}

MuVector MultiplicationCross(const MuVector& a, const MuVector& b)
{
	return MuVector(
		a.Y * b.Z - a.Z * b.Y,
		a.Z * b.X - a.X * b.Z,
		a.X * b.Y - a.Y * b.X,
		1);
}

MuVector operator*(float k, const MuVector& v)
{
	return MuVector(k * v.X, k * v.Y, k * v.Z, 1);
}

float Distance(const MuVector& a, const MuVector& b)
{
	float dx = a.X - b.X;
	float dy = a.Y - b.Y;
	float dz = a.Z - b.Z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}
