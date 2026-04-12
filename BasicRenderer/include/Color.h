#pragma once

#include <cmath>
#include "Vector3.h"

namespace BasicRenderer
{
	class Color
	{
	public:
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

	public:
		Color() noexcept : r(0.0f), g(0.0f), b(0.0f) {}
		Color(float r_, float g_, float b_) noexcept : r(r_), g(g_), b(b_) {}
		Color(const Color&) noexcept = default;
		Color(Color&&) noexcept = default;
		Color(const Vector3& v) noexcept : r(v.x), g(v.y), b(v.z) {}
		~Color() noexcept = default;

		inline Color Normalize() const noexcept
		{
			float l = Length();
			l = 1.f / l;
			return (*this) * l;
		}

		inline float Length() const noexcept { return std::sqrt((r * r) + (g * g) + (b * b)); }
		inline float SquaredLength() const noexcept { return (r * r) + (g * g) + (b * b); }

		Color& operator=(const Color&) noexcept = default;
		Color& operator=(Color&&) noexcept = default;
		bool	operator==(const Color& v) const noexcept;
		bool	operator!=(const Color& v) const noexcept;

		inline Color operator+(const Color& v) const noexcept { return Color(r + v.r, g + v.g, b + v.b); }
		inline Color operator+(const float f) const noexcept { return Color(r + f, g + f, b + f); }
		inline Color operator-(const Color& v) const noexcept { return Color(r - v.r, g - v.g, b - v.b); }
		inline Color operator-(const float f) const noexcept { return Color(r - f, g - f, b - f); }
		inline Color operator*(const Color& v) const noexcept { return Color(r * v.r, g * v.g, b * v.b); }
		inline Color operator*(const float f) const noexcept { return Color(r * f, g * f, b * f); }
		inline Color operator/(const Color& v) const noexcept { return Color(r / v.r, g / v.g, b / v.b); }
		inline Color operator/(const float f) const noexcept { return Color(r / f, g / f, b / f); }

		inline Color& operator+=(const Color& v) noexcept { r += v.r; g += v.g; b += v.b; return *this; }
		inline Color& operator+=(const float f) noexcept { r += f; g += f; b += f; return *this; }
		inline Color& operator-=(const Color& v) noexcept { r -= v.r; g -= v.g; b -= v.b; return *this; }
		inline Color& operator-=(const float f) noexcept { r -= f; g -= f; b -= f; return *this; }
		inline Color& operator*=(const Color& v) noexcept { r *= v.r; g *= v.g; b *= v.b; return *this; }
		inline Color& operator*=(const float f) noexcept { r *= f; g *= f; b *= f; return *this; }
		inline Color& operator/=(const Color& v) noexcept { r /= v.r; g /= v.g; b /= v.b; return *this; }
		inline Color& operator/=(const float f) noexcept { r /= f; g /= f; b /= f; return *this; }


		inline static Color Black() noexcept { return Color(0.0f, 0.0f, 0.0f); }
		inline static Color White() noexcept { return Color(1.0f, 1.0f, 1.0f); }

	};
}