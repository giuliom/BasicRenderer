#include "Vector4.h"


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
