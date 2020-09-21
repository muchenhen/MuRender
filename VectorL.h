#pragma once
#include <cmath>
struct Vec4 
{
	float x, y, z, w;//w=0表示向量，w=1表示点
	Vec4() {}
	Vec4(float xx,float yy):x(xx),y(yy),z(0),w(0){} //表示纹理坐标uv
	Vec4(float xx, float  yy, float zz, float ww=0) :x(xx), y(yy), z(zz), w(ww) {};//可表示向量、颜色
	Vec4 operator +(const Vec4& rhs) const { return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, 0.f); }
	Vec4 operator-() { return Vec4(-x, -y, -z, -w); }
	Vec4 operator-(const Vec4& rhs) const { return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w-rhs.w); }
	Vec4 operator*(const float& r) const { return Vec4(x*r, y*r, z*r, w*r); }
	Vec4 operator*(const Vec4& rhs) const { return Vec4(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w); }
	float Dot(const Vec4 & rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }//点积
	Vec4 Cross(const Vec4& rhs) const { return Vec4(y*rhs.z - z * rhs.y, z*rhs.x - x * rhs.z, x*rhs.y - y * rhs.x, 0.f); }//叉积
	float Norm() const { return x * x + y * y + z * z; }
	float Length() const { return sqrtf(Norm()); }
	Vec4 Normalize() const { float factor = 1.0f / Length(); return Vec4(x*factor, y*factor, z*factor, 0.f); }//单位向量
	//inline Vec4 Lerp(const Vec4& left, const Vec4& right, float t) { return left + (right - left)*t; } //interpolate
	//we use edgefunction to interpolate
};//use Vec4 to representing position/direction/color/ etc