#include "Color.h"

namespace BasicRenderer
{
	bool Color::operator==(const Color& v) const
	{
		return (r == v.r && g == v.g && b == v.b);
	}

	bool Color::operator!=(const Color& v) const
	{
		return !(r == v.r && g == v.g && b == v.b);
	}
}