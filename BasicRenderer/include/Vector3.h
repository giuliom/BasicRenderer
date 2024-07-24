#pragma once

#include <cmath>
#include "Vector4.h"

namespace BasicRenderer
{
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
		Vector3(Vector3&& v) : x(v.x), y(v.y), z(v.z) {}
		Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z) {}
		~Vector3() {}

		inline Vector3 Normalize() const
		{
			float l = Length();
			l = 1.f / l;
			return (*this) * l;
		}

		inline float Length() const { return sqrtf((x * x) + (y * y) + (z * z)); }
		inline float SquaredLength() const { return (x * x) + (y * y) + (z * z); }

		Vector3& operator=(const Vector3& v);
		Vector3& operator=(Vector3&& v);
		bool	operator==(const Vector3& v) const;
		bool	operator!=(const Vector3& v) const;

		inline Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
		inline Vector3 operator+(const float f) const { return Vector3(x + f, y + f, z + f); }
		inline Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
		inline Vector3 operator-(const float f) const { return Vector3(x - f, y - f, z - f); }
		inline Vector3 operator*(const float f) const { return Vector3(x * f, y * f, z * f); }
		inline Vector3 operator/(const float f) const { return Vector3(x / f, y / f, z / f); }

		inline Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		inline Vector3& operator+=(const float f) { x += f; y += f; z += f; return *this; }
		inline Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		inline Vector3& operator-=(const float f) { x -= f; y -= f; z -= f; return *this; }
		inline Vector3& operator*=(const float f) { x *= f; y *= f; z *= f; return *this; }
		inline Vector3& operator/=(const float f) { x /= f; y /= f; z /= f; return *this; }

		inline static float Dot(const Vector3& a, const Vector3& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		inline static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2)
		{
			return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
		}

		inline static Vector3 Min(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
		}

		inline static Vector3 Max(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
		}

		inline static Vector3 Reflect(const Vector3& v, const Vector3& n)
		{
			return v - n * 2 * Dot(v, n);
		}

		inline static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
		inline static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }

	};

	inline Vector3 CalculateNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2) { return Vector3::CrossProduct(v1 - v0, v2 - v0).Normalize(); }
}