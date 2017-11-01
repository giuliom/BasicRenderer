#pragma once

class Vector2
{
public:
	float x = 0.0f;
	float y = 0.0f;

public:
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}
	~Vector2();

	Vector2 Normalize() const;
	float Magnitude() const;

	Vector2& operator=(const Vector2& v);
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator+(const float f) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator-(const float f) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator*(const float f) const;
	bool	operator==(const Vector2& v) const;
	bool	operator!=(const Vector2& v) const;

	static float Dot(const Vector2& a, const Vector2& b);
	static Vector2 Min(const Vector2& a, const Vector2& b);
	static Vector2 Max(const Vector2& a, const Vector2& b);

	static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
	static Vector2 One() { return Vector2(1.0f, 1.0f); }
	
};