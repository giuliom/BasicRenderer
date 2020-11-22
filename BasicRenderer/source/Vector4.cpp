#include "Vector4.h"

namespace BasicRenderer
{
	Vector4& Vector4::operator=(const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	Vector4& Vector4::operator=(Vector4&& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	bool Vector4::operator==(const Vector4& v) const
	{
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}

	bool Vector4::operator!=(const Vector4& v) const
	{
		return !(x == v.x && y == v.y && z == v.z && w == v.w);
	}
}