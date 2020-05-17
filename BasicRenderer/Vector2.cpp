#include "Vector2.h"

namespace BasicRenderer
{
	Vector2& Vector2::operator=(const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	Vector2& Vector2::operator=(Vector2&& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	bool Vector2::operator==(const Vector2& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool Vector2::operator!=(const Vector2& v) const
	{
		return !(x == v.x && y == v.y);
	}
}