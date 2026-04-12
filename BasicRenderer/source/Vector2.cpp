#include "Vector2.h"

namespace BasicRenderer
{
	bool Vector2::operator==(const Vector2& v) const noexcept
	{
		return (x == v.x && y == v.y);
	}

	bool Vector2::operator!=(const Vector2& v) const noexcept
	{
		return !(x == v.x && y == v.y);
	}
}