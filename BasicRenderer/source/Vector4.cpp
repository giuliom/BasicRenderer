#include "Vector4.h"

namespace BasicRenderer
{
	bool Vector4::operator==(const Vector4& v) const
	{
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}

	bool Vector4::operator!=(const Vector4& v) const
	{
		return !(x == v.x && y == v.y && z == v.z && w == v.w);
	}
}