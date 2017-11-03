#pragma once

#include "Vector4.h"

class Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

public:
	Vector3() {}
	Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z) {}
	~Vector3();

	Vector3 Normalize() const;
	float Magnitude() const;

	Vector3& operator=(const Vector3& v);
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator+(const float f) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator-(const float f) const;
	Vector3 operator*(const Vector3& v) const;
	Vector3 operator*(const float f) const;
	bool	operator==(const Vector3& v) const;
	bool	operator!=(const Vector3& v) const;

	static float Dot(const Vector3& a, const Vector3& b);
	static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);
	static Vector3 Min(const Vector3& a, const Vector3& b);
	static Vector3 Max(const Vector3& a, const Vector3& b);
		
	static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
	static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }

};