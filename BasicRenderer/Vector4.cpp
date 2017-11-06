#include "Vector4.h"
#include <math.h>



Vector4::~Vector4()
{
}

Vector4 Vector4::Normalize() const
{
	float l = 1.0f / Magnitude();
	if (l == 0) return Vector4::Zero();
	return Vector4(x * l, y * l, z * l, w * l);
}

float Vector4::Magnitude() const
{
	return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
}

Vector4& Vector4::operator=(const Vector4 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

Vector4 & Vector4::operator=(Vector4 && v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

Vector4 Vector4::operator+(const Vector4 & v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator+(const float f) const
{
	return Vector4(x + f, y +f, z + f, w + f);
}

Vector4 Vector4::operator-(const Vector4 & v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator-(const float f) const
{
	return Vector4(x - f, y - f, z - f, w - f);
}

Vector4 Vector4::operator*(const Vector4 & v) const
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator*(const float f) const
{
	return Vector4(x * f, y * f, z * f, w * f);
}

bool Vector4::operator==(const Vector4 & v) const
{
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}

bool Vector4::operator!=(const Vector4 & v) const
{
	return !(x == v.x && y == v.y && z == v.z && w == v.w);
}

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
	Vector4 an = a.Normalize();
	Vector4 bn = b.Normalize();
		
	return an.x * bn.x + an.y * bn.y + an.z * bn.z + an.w * bn.w;
}

Vector4 Vector4::Min(const Vector4& a, const Vector4& b)
{
	Vector4 v;
	v.x = a.x < b.x ? a.x : b.x;
	v.y = a.y < b.y ? a.y : b.y;
	v.z = a.z < b.z ? a.z : b.z;
	v.w = a.w < b.w ? a.w : b.w;
	return v;
}

Vector4 Vector4::Max(const Vector4& a, const Vector4& b)
{
	Vector4 v;
	v.x = a.x > b.x ? a.x : b.x;
	v.y = a.y > b.y ? a.y : b.y;
	v.z = a.z > b.z ? a.z : b.z;
	v.w = a.w > b.w ? a.w : b.w;
	return v;
}
