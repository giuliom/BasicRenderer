#pragma once

#include <cassert>
#include "Vector4.h"
#include "Vector3.h"

namespace BasicRenderer
{
	/* Row-major */
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
		Matrix4(Matrix4&& m);
		~Matrix4() {}

		Matrix4& operator=(const Matrix4& m);
		Matrix4& operator=(Matrix4&& m);

		inline Vector4 operator*(const Vector4& v) const
		{
			return Vector4(
				x1 * v.x + x2 * v.y + x3 * v.z + x4 * v.w,
				y1 * v.x + y2 * v.y + y3 * v.z + y4 * v.w,
				z1 * v.x + z2 * v.y + z3 * v.z + z4 * v.w,
				w1 * v.x + w2 * v.y + w3 * v.z + w4 * v.w
			);
		}

		inline Matrix4 operator*(const float f) const
		{
			return Matrix4(x1 * f, x2 * f, x3 * f, x4 * f,
				y1 * f, y2 * f, y3 * f, y4 * f,
				z1 * f, z2 * f, z3 * f, z4 * f,
				w1 * f, w2 * f, w3 * f, w4 * f
			);
		}

		inline Vector3 operator*(const Vector3& v) const
		{
			return Vector3(
				x1 * v.x + x2 * v.y + x3 * v.z + x4 * 1.0f,
				y1 * v.x + y2 * v.y + y3 * v.z + y4 * 1.0f,
				z1 * v.x + z2 * v.y + z3 * v.z + z4 * 1.0f
			);
		}

		inline float Det() const
		{
			return x1 * y2 * z3 * w4 + x1 * y3 * z4 * w2 + x1 * y4 * z2 * w3
				+ x2 * y1 * z4 * w3 + x2 * y3 * z1 * w4 + x2 * y4 * z3 * w1
				+ x3 * y1 * z2 * w4 + x3 * y2 * z4 * w1 + x3 * y4 * z1 * w2
				+ x4 * y1 * z3 * w2 + x4 * y2 * z1 * w3 + x4 * y3 * z2 * w1
				- x1 * y2 * z4 * w3 - x1 * y3 * z2 * w4 - x1 * y4 * z3 * w2
				- x2 * y1 * z3 * w4 - x2 * y3 * z4 * w1 - x2 * y4 * z1 * w3
				- x3 * y1 * z4 * w2 - x3 * y2 * z1 * w4 - x3 * y4 * z2 * w1
				- x4 * y1 * z2 * w3 - x4 * y2 * z3 * w1 - x4 * y3 * z1 * w2;
		}

		Matrix4 Inverse() const;

		inline static Matrix4 Identity()
		{
			return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}

		inline static Matrix4 Zero()
		{
			return Matrix4(
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
			);
		}
	};

	Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
}