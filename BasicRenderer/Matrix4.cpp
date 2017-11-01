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

Matrix4::~Matrix4()
{
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

Matrix4 Matrix4::operator*(const Matrix4 & m) const
{
	return Matrix4(	x1*m.x1 + x2*m.y1 + x3*m.z1 + x4*m.w1,
					x1*m.x2 + x2*m.y2 + x3*m.z2 + x4*m.w2,
					x1*m.x3 + x2*m.y3 + x3*m.z3 + x4*m.w3,
					x1*m.x4 + x2*m.y4 + x3*m.z4 + x4*m.w4,

					y1*m.x1 + y2*m.y1 + y3*m.z1 + y4*m.w1,
					y1*m.x2 + y2*m.y2 + y3*m.z2 + y4*m.w2,
					y1*m.x3 + y2*m.y3 + y3*m.z3 + y4*m.w3,
					y1*m.x4 + y2*m.y4 + y3*m.z4 + y4*m.w4,

					z1*m.x1 + z2*m.y1 + z3*m.z1 + z4*m.w1,
					z1*m.x2 + z2*m.y2 + z3*m.z2 + z4*m.w2,
					z1*m.x3 + z2*m.y3 + z3*m.z3 + z4*m.w3,
					z1*m.x4 + z2*m.y4 + z3*m.z4 + z4*m.w4,

					w1*m.x1 + w2*m.y1 + w3*m.z1 + w4*m.w1,
					w1*m.x2 + w2*m.y2 + w3*m.z2 + w4*m.w2,
					w1*m.x3 + w2*m.y3 + w3*m.z3 + w4*m.w3,
					w1*m.x4 + w2*m.y4 + w3*m.z4 + w4*m.w4
				);

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

Matrix4 Matrix4::Identity()
{
	return Matrix4( 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 1.0f, 0.0f, 
					0.0f, 0.0f, 0.0f, 1.0f );
}

Matrix4 Matrix4::Zero()
{
	return Matrix4(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
}
