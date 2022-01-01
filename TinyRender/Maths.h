#pragma once
#include <ostream>
template <class T> 
struct Vector3
{
	T x;
	T y;
	T z;

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(T x1, T y1, T z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}

	inline Vector3<T> operator  + (const Vector3<T> &Vec3)
	{
		return Vector3<T>(x + Vec3.x, y + Vec3.y, z + Vec3.z);
	}

	inline Vector3<T> operator  - (const Vector3<T> &Vec3)
	{
		return Vector3<T>(x - Vec3.x, y - Vec3.y, z - Vec3.z);
	}

	inline Vector3<T> operator  * (const Vector3<T> &Vec3)
	{
		return Vector3<T>(x * Vec3.x, y * Vec3.y, z * Vec3.z);
	}
};

template <class T> 
std::ostream& operator<<(std::ostream& s, Vector3<T>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}