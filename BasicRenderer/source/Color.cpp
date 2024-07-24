#include "Color.h"

namespace BasicRenderer
{
	Color& Color::operator=(const Color& v)
	{
		r = v.r;
		g = v.g;
		b = v.b;
		return *this;
	}

	Color& Color::operator=(Color&& v)
	{
		r = v.r;
		g = v.g;
		b = v.b;
		return *this;
	}

	bool Color::operator==(const Color& v) const
	{
		return (r == v.r && g == v.g && b == v.b);
	}

	bool Color::operator!=(const Color& v) const
	{
		return !(r == v.r && g == v.g && b == v.b);
	}
}