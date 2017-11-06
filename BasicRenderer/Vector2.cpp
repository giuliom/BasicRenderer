#include "Vector2.h"
#include <math.h>



Vector2::~Vector2()
{
}

Vector2 Vector2::Normalize() const
{
	float l = 1.0f / Magnitude();
	if (l == 0) return Vector2::Zero();
	return Vector2(x * l, y * l);
}

float Vector2::Magnitude() const
{
	return sqrtf((x * x) + (y * y));
}

Vector2& Vector2::operator=(const Vector2 & v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Vector2 & Vector2::operator=(Vector2 && v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Vector2 Vector2::operator+(const Vector2 & v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator+(const float f) const
{
	return Vector2(x + f, y + f);
}

Vector2 Vector2::operator-(const Vector2 & v) const
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator-(const float f) const
{
	return Vector2(x - f, y - f);
}

Vector2 Vector2::operator*(const Vector2 & v) const
{
	return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator*(const float f) const
{
	return Vector2(x * f, y * f);
}

bool Vector2::operator==(const Vector2 & v) const
{
	return (x == v.x && y == v.y) ;
}

bool Vector2::operator!=(const Vector2 & v) const
{
	return !(x == v.x && y == v.y);
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	Vector2 an = a.Normalize();
	Vector2 bn = b.Normalize();
		
	return an.x * bn.x + an.y * bn.y;
}

Vector2 Vector2::Min(const Vector2& a, const Vector2& b)
{
	Vector2 v;
	v.x = a.x < b.x ? a.x : b.x;
	v.y = a.y < b.y ? a.y : b.y;
	return v;
}

Vector2 Vector2::Max(const Vector2& a, const Vector2& b)
{
	Vector2 v;
	v.x = a.x > b.x ? a.x : b.x;
	v.y = a.y > b.y ? a.y : b.y;
	return v;
}
