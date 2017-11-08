#include "Vector3.h"

Vector3& Vector3::operator=(const Vector3 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3 & Vector3::operator=(Vector3 && v)
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


