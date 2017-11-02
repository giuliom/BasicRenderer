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

	Matrix4(float x1_, float x2_, float x3_, float x4_,
			float y1_, float y2_, float y3_, float y4_,
			float z1_, float z2_, float z3_, float z4_,
			float w1_, float w2_, float w3_, float w4_) : 
		x1(x1_), x2(x2_), x3(x3_), x4(x4_),
		y1(y1_), y2(y2_), y3(y3_), y4(y4_),
		z1(z1_), z2(z2_), z3(z3_), z4(z4_),
		w1(w1_), w2(w2_), w3(w3_), w4(w4_) {}
	
	
	Matrix4(const Matrix4& m);
	~Matrix4();

	Matrix4& operator=(const Matrix4& m);
	Matrix4 operator*(const Matrix4& m) const;
	Vector4 operator*(const Vector4& v) const;
	Matrix4 operator*(const float f) const;

	static Matrix4 Identity();
	static Matrix4 Zero();
};

