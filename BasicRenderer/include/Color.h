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
		Color() {}
		Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}
		Color(const Color& v) : r(v.r), g(v.g), b(v.b) {}
		Color(Color&& v) : r(v.r), g(v.g), b(v.b) {}
		Color(const Vector3& v) : r(v.x), g(v.y), b(v.z) {}
		~Color() {}

		inline Color Normalize() const
		{
			float l = Length();
			l = 1.f / l;
			return (*this) * l;
		}

		inline float Length() const { return sqrtf((r * r) + (g * g) + (b * b)); }
		inline float SquaredLength() const { return (r * r) + (g * g) + (b * b); }

		Color& operator=(const Color& v);
		Color& operator=(Color&& v);
		bool	operator==(const Color& v) const;
		bool	operator!=(const Color& v) const;

		inline Color operator+(const Color& v) const { return Color(r + v.r, g + v.g, b + v.b); }
		inline Color operator+(const float f) const { return Color(r + f, g + f, b + f); }
		inline Color operator-(const Color& v) const { return Color(r - v.r, g - v.g, b - v.b); }
		inline Color operator-(const float f) const { return Color(r - f, g - f, b - f); }
		inline Color operator*(const Color& v) const { return Color(r * v.r, g * v.g, b * v.b); }
		inline Color operator*(const float f) const { return Color(r * f, g * f, b * f); }
		inline Color operator/(const Color& v) const { return Color(r / v.r, g / v.g, b / v.b); }
		inline Color operator/(const float f) const { return Color(r / f, g / f, b / f); }

		inline Color& operator+=(const Color& v) { r += v.r; g += v.g; b += v.b; return *this; }
		inline Color& operator+=(const float f) { r += f; g += f; b += f; return *this; }
		inline Color& operator-=(const Color& v) { r -= v.r; g -= v.g; b -= v.b; return *this; }
		inline Color& operator-=(const float f) { r -= f; g -= f; b -= f; return *this; }
		inline Color& operator*=(const Color& v) { r *= v.r; g *= v.g; b *= v.b; return *this; }
		inline Color& operator*=(const float f) { r *= f; g *= f; b *= f; return *this; }
		inline Color& operator/=(const Color& v) { r /= v.r; g /= v.g; b /= v.b; return *this; }
		inline Color& operator/=(const float f) { r /= f; g /= f; b /= f; return *this; }


		inline static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
		inline static Color White() { return Color(1.0f, 1.0f, 1.0f); }

	};
}