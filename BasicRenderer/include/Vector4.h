#pragma once

#include <cmath>

namespace BasicRenderer
{
	class Vector4
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

	public:
		Vector4() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Vector4(float x_, float y_, float z_, float w_) noexcept : x(x_), y(y_), z(z_), w(w_) {}
		Vector4(const Vector4&) noexcept = default;
		Vector4(Vector4&&) noexcept = default;
		~Vector4() noexcept = default;

		inline Vector4 Normalize() const noexcept
		{
			float l = Length();
			l = 1.0f / l;
			return Vector4(x * l, y * l, z * l, w * l);
		}

		inline float Length() const noexcept { return std::sqrt((x * x) + (y * y) + (z * z) + (w * w)); }

		Vector4& operator=(const Vector4&) noexcept = default;
		Vector4& operator=(Vector4&&) noexcept = default;
		bool	operator==(const Vector4& v) const noexcept;
		bool	operator!=(const Vector4& v) const noexcept;

		inline Vector4 operator+(const Vector4& v) const noexcept { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
		inline Vector4 operator+(const float f) const noexcept { return Vector4(x + f, y + f, z + f, w + f); }
		inline Vector4 operator-(const Vector4& v) const noexcept { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
		inline Vector4 operator-(const float f) const noexcept { return Vector4(x - f, y - f, z - f, w - f); }
		inline Vector4 operator*(const float f) const noexcept { return Vector4(x * f, y * f, z * f, w * f); }
		inline Vector4 operator/(const float f) const noexcept { return Vector4(x / f, y / f, z / f, w / f); }

		inline Vector4& operator+=(const Vector4& v) noexcept { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		inline Vector4& operator+=(const float f) noexcept { x += f; y += f; z += f; w += f; return *this; }
		inline Vector4& operator-=(const Vector4& v) noexcept { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		inline Vector4& operator-=(const float f) noexcept { x -= f; y -= f; z -= f; w -= f; return *this; }
		inline Vector4& operator*=(const float f) noexcept { x *= f; y *= f; z *= f; w *= f; return *this; }
		inline Vector4& operator/=(const float f) noexcept { x /= f; y /= f; z /= f; w /= f; return *this; }

		inline static float Dot(const Vector4& a, const Vector4& b) noexcept
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		inline static Vector4 Min(const Vector4& a, const Vector4& b) noexcept
		{
			return Vector4(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z, a.w < b.w ? a.w : b.w);
		}

		inline static Vector4 Max(const Vector4& a, const Vector4& b) noexcept
		{
			return Vector4(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z, a.w > b.w ? a.w : b.w);
		}

		inline static Vector4 Zero() noexcept { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
		inline static Vector4 One() noexcept { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }

	};
}