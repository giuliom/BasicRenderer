#include "Vector3.h"
#include <math.h>


Vector3::~Vector3()
{
}

Vector3 Vector3::Normalize() const
{
	float l = 1 / Magnitude();
	if (l == 0) return Vector3::Zero();
	return Vector3(x * l, y * l, z * l);
}

float Vector3::Magnitude() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

Vector3& Vector3::operator=(const Vector3 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3 Vector3::operator+(const Vector3 & v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator+(const float f) const
{
	return Vector3(x + f, y + f, z + f);
}

Vector3 Vector3::operator-(const Vector3 & v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-(const float f) const
{
	return Vector3(x - f, y - f, z - f);
}

Vector3 Vector3::operator*(const Vector3 & v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator*(const float f) const
{
	return Vector3(x * f, y * f, z * f);
}

bool Vector3::operator==(const Vector3 & v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

bool Vector3::operator!=(const Vector3 & v) const
{
	return !(x == v.x && y == v.y && z == v.z);
}

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	Vector3 an = a.Normalize();
	Vector3 bn = b.Normalize();
		
		return an.x * bn.x + an.y * bn.y + an.z * bn.z;
}

Vector3 Vector3::CrossProduct(const Vector3& v1, const Vector3& v2)
{
	float vx = v1.y * v2.z - v1.z * v2.y;
	float vy = v1.z * v2.x - v1.x * v2.z;
	float vz = v1.x * v2.y - v1.y * v2.x;
	return Vector3(vx, vy, vz);
}

Vector3 Vector3::Min(const Vector3& a, const Vector3& b)
{
	Vector3 v;
	v.x = a.x < b.x ? a.x : b.x;
	v.y = a.y < b.y ? a.y : b.y;
	v.z = a.z < b.z ? a.z : b.z;
	return v;
}

Vector3 Vector3::Max(const Vector3& a, const Vector3& b)
{
	Vector3 v;
	v.x = a.x > b.x ? a.x : b.x;
	v.y = a.y > b.y ? a.y : b.y;
	v.z = a.z > b.z ? a.z : b.z;
	return v;
}
