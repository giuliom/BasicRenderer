#include "ImageFormats.h"

RGB888 & RGB888::operator=(RGB888 & other)
{
	r = other.r;
	g = other.g;
	b = other.b;

	return *this;
}
