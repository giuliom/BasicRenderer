#pragma once

#include <cmath>

namespace BasicRenderer
{
	class Vector2
	{
	public:
		float x = 0.0f;
		float y = 0.0f;

	public:
		Vector2() noexcept : x(0.0f), y(0.0f) {}
		Vector2(float x_, float y_) noexcept : x(x_), y(y_) {}
		Vector2(const Vector2&) noexcept = default;
		Vector2(Vector2&&) noexcept = default;
		~Vector2() noexcept = default;

		inline Vector2 Normalize() const noexcept
		{
			float l = Length();
			l = 1.0f / l;
			return Vector2(x * l, y * l);
		}

		inline float Length() const noexcept
		{
			return std::sqrt((x * x) + (y * y));
		}

		Vector2& operator=(const Vector2&) noexcept = default;
		Vector2& operator=(Vector2&&) noexcept = default;
		bool	operator==(const Vector2& v) const noexcept;
		bool	operator!=(const Vector2& v) const noexcept;

		inline Vector2 operator+(const Vector2& v) const noexcept { return Vector2(x + v.x, y + v.y); }
		inline Vector2 operator+(const float f) const noexcept { return Vector2(x + f, y + f); }
		inline Vector2 operator-(const Vector2& v) const noexcept { return Vector2(x - v.x, y - v.y); }
		inline Vector2 operator-(const float f) const noexcept { return Vector2(x - f, y - f); }
		inline Vector2 operator*(const float f) const noexcept { return Vector2(x * f, y * f); }
		inline Vector2 operator/(const float f) const noexcept { const float inv = 1.0f / f; return Vector2(x * inv, y * inv); }

		inline Vector2& operator+=(const Vector2& v) noexcept { x += v.x; y += v.y; return *this; }
		inline Vector2& operator+=(const float f) noexcept { x += f; y += f; return *this; }
		inline Vector2& operator-=(const Vector2& v) noexcept { x -= v.x; y -= v.y; return *this; }
		inline Vector2& operator-=(const float f) noexcept { x -= f; y -= f; return *this; }
		inline Vector2& operator*=(const float f) noexcept { x *= f; y *= f; return *this; }
		inline Vector2& operator/=(const float f) noexcept { const float inv = 1.0f / f; x *= inv; y *= inv; return *this; }

		inline static float Dot(const Vector2& a, const Vector2& b) noexcept
		{
			return a.x * b.x + a.y * b.y;
		}

		inline static Vector2 Min(const Vector2& a, const Vector2& b) noexcept
		{
			return Vector2(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y);
		}

		inline static Vector2 Max(const Vector2& a, const Vector2& b) noexcept
		{
			return Vector2(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y);
		}

		inline static Vector2 Zero() noexcept { return Vector2(0.0f, 0.0f); }
		inline static Vector2 One() noexcept { return Vector2(1.0f, 1.0f); }

	};
}