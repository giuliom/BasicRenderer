#pragma once

#include "Vector4.h"

class Matrix4
{
public:
	float x1 = 0.0f;
	float x2 = 0.0f;
	float x3 = 0.0f;
	float x4 = 0.0f;
	float y1 = 0.0f;
	float y2 = 0.0f;
	float y3 = 0.0f;
	float y4 = 0.0f;
	float z1 = 0.0f;
	float z2 = 0.0f;
	float z3 = 0.0f;
	float z4 = 0.0f;
	float w1 = 0.0f;
	float w2 = 0.0f;
	float w3 = 0.0f;
	float w4 = 0.0f;

public:
	Matrix4() : 
		x1(1.0f), x2(0.0f), x3(0.0f), x4(0.0f),
		y1(0.0f), y2(1.0f), y3(0.0f), y4(0.0f),
		z1(0.0f), z2(0.0f), z3(1.0f), z4(0.0f),
		w1(0.0f), w2(0.0f), w3(0.0f), w4(1.0f) {}

	Matrix4(float x1, float x2, float x3, float x4,
			float y1, float y2, float y3, float y4,
			float z1, float z2, float z3, float z4,
			float w1, float w2, float w3, float w4) : 
		x1(x1), x2(x2), x3(x3), x4(x4),
		y1(y1), y2(y2), y3(y3), y4(y4),
		z1(z1), z2(z2), z3(z3), z4(z4),
		w1(w1), w2(w2), w3(w3), w4(w4) {}
	
	
	Matrix4(const Matrix4& m);
	~Matrix4();

	Matrix4& operator=(const Matrix4& m);
	Matrix4 operator*(const Matrix4& m) const;
	Vector4 operator*(const Vector4& v) const;
	Matrix4 operator*(const float f) const;

	static Matrix4 Identity();
	static Matrix4 Zero();
};

