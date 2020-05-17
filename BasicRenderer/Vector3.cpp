#include "Vector3.h"

namespace BasicRenderer
{
	Vector3& Vector3::operator=(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3& Vector3::operator=(Vector3&& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	bool Vector3::operator==(const Vector3& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	bool Vector3::operator!=(const Vector3& v) const
	{
		return !(x == v.x && y == v.y && z == v.z);
	}
}

