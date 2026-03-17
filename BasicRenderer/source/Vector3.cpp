#include "Vector3.h"

namespace BasicRenderer
{
	bool Vector3::operator==(const Vector3& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	bool Vector3::operator!=(const Vector3& v) const
	{
		return !(x == v.x && y == v.y && z == v.z);
	}
}

