#include "Matrix4.h"

Matrix4::Matrix4(const Matrix4 & m)
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

Matrix4::Matrix4(Matrix4 && m)
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

Matrix4& Matrix4::operator=(const Matrix4 & m)
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

Matrix4 & Matrix4::operator=(Matrix4 && m)
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

Vector4 Matrix4::operator*(const Vector4 & v) const
{
	return Vector4(
		x1*v.x + x2*v.y + x3*v.z + x4*v.w,
		y1*v.x + y2*v.y + y3*v.z + y4*v.w,
		z1*v.x + z2*v.y + z3*v.z + z4*v.w,
		w1*v.x + w2*v.y + w3*v.z + w4*v.w
	);
}

Matrix4 Matrix4::operator*(const float f) const
{
	return Matrix4( x1 * f, x2 * f, x3 * f, x4 * f,
					y1 * f, y2 * f, y3 * f, y4 * f,
					z1 * f, z2 * f, z3 * f, z4 * f,
					w1 * f, w2 * f, w3 * f, w4 * f
	);
}

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	return Matrix4( m1.x1*m2.x1 + m1.x2*m2.y1 + m1.x3*m2.z1 + m1.x4*m2.w1,
					m1.x1*m2.x2 + m1.x2*m2.y2 + m1.x3*m2.z2 + m1.x4*m2.w2,
					m1.x1*m2.x3 + m1.x2*m2.y3 + m1.x3*m2.z3 + m1.x4*m2.w3,
					m1.x1*m2.x4 + m1.x2*m2.y4 + m1.x3*m2.z4 + m1.x4*m2.w4,

					m1.y1*m2.x1 + m1.y2*m2.y1 + m1.y3*m2.z1 + m1.y4*m2.w1,
					m1.y1*m2.x2 + m1.y2*m2.y2 + m1.y3*m2.z2 + m1.y4*m2.w2,
					m1.y1*m2.x3 + m1.y2*m2.y3 + m1.y3*m2.z3 + m1.y4*m2.w3,
					m1.y1*m2.x4 + m1.y2*m2.y4 + m1.y3*m2.z4 + m1.y4*m2.w4,

					m1.z1*m2.x1 + m1.z2*m2.y1 + m1.z3*m2.z1 + m1.z4*m2.w1,
					m1.z1*m2.x2 + m1.z2*m2.y2 + m1.z3*m2.z2 + m1.z4*m2.w2,
					m1.z1*m2.x3 + m1.z2*m2.y3 + m1.z3*m2.z3 + m1.z4*m2.w3,
					m1.z1*m2.x4 + m1.z2*m2.y4 + m1.z3*m2.z4 + m1.z4*m2.w4,

					m1.w1*m2.x1 + m1.w2*m2.y1 + m1.w3*m2.z1 + m1.w4*m2.w1,
					m1.w1*m2.x2 + m1.w2*m2.y2 + m1.w3*m2.z2 + m1.w4*m2.w2,
					m1.w1*m2.x3 + m1.w2*m2.y3 + m1.w3*m2.z3 + m1.w4*m2.w3,
					m1.w1*m2.x4 + m1.w2*m2.y4 + m1.w3*m2.z4 + m1.w4*m2.w4
				);
}
