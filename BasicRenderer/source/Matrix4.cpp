#include "Matrix4.h"

namespace BasicRenderer
{
	Matrix4::Matrix4(const Matrix4& m)
	{
		x1 = m.x1;
		x2 = m.x2;
		x3 = m.x3;
		x4 = m.x4;
		y1 = m.y1;
		y2 = m.y2;
		y3 = m.y3;
		y4 = m.y4;
		z1 = m.z1;
		z2 = m.z2;
		z3 = m.z3;
		z4 = m.z4;
		w1 = m.w1;
		w2 = m.w2;
		w3 = m.w3;
		w4 = m.w4;
	}

	Matrix4::Matrix4(Matrix4&& m)
	{
		x1 = m.x1;
		x2 = m.x2;
		x3 = m.x3;
		x4 = m.x4;
		y1 = m.y1;
		y2 = m.y2;
		y3 = m.y3;
		y4 = m.y4;
		z1 = m.z1;
		z2 = m.z2;
		z3 = m.z3;
		z4 = m.z4;
		w1 = m.w1;
		w2 = m.w2;
		w3 = m.w3;
		w4 = m.w4;
	}

	Matrix4& Matrix4::operator=(const Matrix4& m)
	{
		x1 = m.x1;
		x2 = m.x2;
		x3 = m.x3;
		x4 = m.x4;
		y1 = m.y1;
		y2 = m.y2;
		y3 = m.y3;
		y4 = m.y4;
		z1 = m.z1;
		z2 = m.z2;
		z3 = m.z3;
		z4 = m.z4;
		w1 = m.w1;
		w2 = m.w2;
		w3 = m.w3;
		w4 = m.w4;
		return *this;
	}

	Matrix4& Matrix4::operator=(Matrix4&& m)
	{
		x1 = m.x1;
		x2 = m.x2;
		x3 = m.x3;
		x4 = m.x4;
		y1 = m.y1;
		y2 = m.y2;
		y3 = m.y3;
		y4 = m.y4;
		z1 = m.z1;
		z2 = m.z2;
		z3 = m.z3;
		z4 = m.z4;
		w1 = m.w1;
		w2 = m.w2;
		w3 = m.w3;
		w4 = m.w4;
		return *this;
	}

	Matrix4 Matrix4::Inverse() const
	{
		float det = Det();
		assert(det != 0);

		Matrix4 m = {

			y2 * z3 * w4 + y3 * z4 * w2 + y4 * z2 * w3 - y2 * z4 * w3 - y3 * z2 * w4 - y4 * z3 * w2,
			x2 * z4 * w3 + x3 * z2 * w4 + x4 * z3 * w2 - x2 * z3 * w4 - x3 * z4 * w2 - x4 * z2 * w3,
			x2 * y3 * w4 + x3 * y4 * w2 + x4 * y2 * w3 - x2 * y4 * w3 - x3 * y2 * w4 - x4 * y3 * w2,
			x2 * y4 * z3 + x3 * y2 * z4 + x4 * y3 * z2 - x2 * y3 * z4 - x3 * y4 * z2 - x4 * y2 * z3,

			y1 * z4 * w3 + y3 * z1 * w4 + y4 * z3 * w1 - y1 * z3 * w4 - y3 * z4 * w1 - y4 * z1 * w3,
			x1 * z3 * w4 + x3 * z4 * w1 + x4 * z1 * w3 - x1 * z4 * w3 - x3 * z1 * w4 - x4 * z3 * w1,
			x1 * y4 * w3 + x3 * y1 * w4 + x4 * y3 * w1 - x1 * y3 * w4 - x3 * y4 * w1 - x4 * y1 * w3,
			x1 * y3 * z4 + x3 * y4 * z1 + x4 * y1 * z3 - x1 * y4 * z3 - x3 * y1 * z4 - x4 * y3 * z1,

			y1 * z2 * w4 + y2 * z4 * w1 + y4 * z1 * w2 - y1 * z4 * w2 - y2 * z1 * w4 - y4 * z2 * w1,
			x1 * z4 * w2 + x2 * z1 * w4 + x4 * z2 * w1 - x1 * z2 * w4 - x2 * z4 * w1 - x4 * z1 * w2,
			x1 * y2 * w4 + x2 * y4 * w1 + x4 * y1 * w2 - x1 * y4 * w2 - x2 * y1 * w4 - x4 * y2 * w1,
			x1 * y4 * z2 + x2 * y1 * z4 + x4 * y2 * z1 - x1 * y2 * z4 - x2 * y4 * z1 - x4 * y1 * z2,

			y1 * z3 * w2 + y2 * z1 * w3 + y3 * z2 * w1 - y1 * z2 * w3 - y2 * z3 * w1 - y3 * z1 * w2,
			x1 * z2 * w3 + x2 * z3 * w1 + x3 * z1 * w2 - x1 * z3 * w2 - x2 * z1 * w3 - x3 * z2 * w1,
			x1 * y3 * w2 + x2 * y1 * w3 + x3 * y2 * w1 - x1 * y2 * w3 - x2 * y3 * w1 - x3 * y1 * w2,
			x1 * y2 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x1 * y3 * z2 - x2 * y1 * z3 - x3 * y2 * z1

		};

		return m * (1.0f / det);
	}


	Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
	{
		return Matrix4(m1.x1 * m2.x1 + m1.x2 * m2.y1 + m1.x3 * m2.z1 + m1.x4 * m2.w1,
			m1.x1 * m2.x2 + m1.x2 * m2.y2 + m1.x3 * m2.z2 + m1.x4 * m2.w2,
			m1.x1 * m2.x3 + m1.x2 * m2.y3 + m1.x3 * m2.z3 + m1.x4 * m2.w3,
			m1.x1 * m2.x4 + m1.x2 * m2.y4 + m1.x3 * m2.z4 + m1.x4 * m2.w4,

			m1.y1 * m2.x1 + m1.y2 * m2.y1 + m1.y3 * m2.z1 + m1.y4 * m2.w1,
			m1.y1 * m2.x2 + m1.y2 * m2.y2 + m1.y3 * m2.z2 + m1.y4 * m2.w2,
			m1.y1 * m2.x3 + m1.y2 * m2.y3 + m1.y3 * m2.z3 + m1.y4 * m2.w3,
			m1.y1 * m2.x4 + m1.y2 * m2.y4 + m1.y3 * m2.z4 + m1.y4 * m2.w4,

			m1.z1 * m2.x1 + m1.z2 * m2.y1 + m1.z3 * m2.z1 + m1.z4 * m2.w1,
			m1.z1 * m2.x2 + m1.z2 * m2.y2 + m1.z3 * m2.z2 + m1.z4 * m2.w2,
			m1.z1 * m2.x3 + m1.z2 * m2.y3 + m1.z3 * m2.z3 + m1.z4 * m2.w3,
			m1.z1 * m2.x4 + m1.z2 * m2.y4 + m1.z3 * m2.z4 + m1.z4 * m2.w4,

			m1.w1 * m2.x1 + m1.w2 * m2.y1 + m1.w3 * m2.z1 + m1.w4 * m2.w1,
			m1.w1 * m2.x2 + m1.w2 * m2.y2 + m1.w3 * m2.z2 + m1.w4 * m2.w2,
			m1.w1 * m2.x3 + m1.w2 * m2.y3 + m1.w3 * m2.z3 + m1.w4 * m2.w3,
			m1.w1 * m2.x4 + m1.w2 * m2.y4 + m1.w3 * m2.z4 + m1.w4 * m2.w4
		);
	}
}