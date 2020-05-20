#pragma once

#include <math.h>

namespace BasicRenderer
{
	class Vector2
	{
	public:
		float x = 0.0f;
		float y = 0.0f;

	public:
		Vector2() : x(0.0f), y(0.0f) {}
		Vector2(float x_, float y_) : x(x_), y(y_) {}
		Vector2(const Vector2& v) : x(v.x), y(v.y) {}
		Vector2(Vector2&& v) : x(v.x), y(v.y) {}
		~Vector2() {}

		inline Vector2 Normalize() const
		{
			float l = Magnitude();
			l = 1.0f / l;
			return Vector2(x * l, y * l);
		}

		inline float Magnitude() const
		{
			return sqrtf((x * x) + (y * y));
		}

		Vector2& operator=(const Vector2& v);
		Vector2& operator=(Vector2&& v);
		bool	operator==(const Vector2& v) const;
		bool	operator!=(const Vector2& v) const;

		inline Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
		inline Vector2 operator+(const float f) const { return Vector2(x + f, y + f); }
		inline Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
		inline Vector2 operator-(const float f) const { return Vector2(x - f, y - f); }
		inline Vector2 operator*(const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
		inline Vector2 operator*(const float f) const { return Vector2(x * f, y * f); }
		inline Vector2 operator/(const Vector2& v) const { return Vector2(x / v.x, y / v.y); }
		inline Vector2 operator/(const float f) const { return Vector2(x / f, y / f); }

		inline static float Dot(const Vector2& a, const Vector2& b)
		{
			Vector2 an = a.Normalize();
			Vector2 bn = b.Normalize();
			return an.x * bn.x + an.y * bn.y;
		}

		inline static Vector2 Min(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y);
		}

		inline static Vector2 Max(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y);
		}

		inline static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
		inline static Vector2 One() { return Vector2(1.0f, 1.0f); }

	};
}