#include "Face.h"

namespace BasicRenderer
{
	Face::Face(const Position& p0, const Position& p1, const  Position& p2, const Face& f)
		: v0(p0, f.v0.nrml, f.v0.uv)
		, v1(p1, f.v1.nrml, f.v1.uv)
		, v2(p2, f.v2.nrml, f.v2.uv)
		, normal(CalculateNormal(v0.pos, v1.pos, v2.pos))
	{
	}

	Face& Face::operator=(const Face& f)
	{
		v0 = f.v0;
		v1 = f.v1;
		v2 = f.v2;
		normal = f.normal;
		return *this;
	}

	Face& Face::operator=(Face&& f)
	{
		v0 = f.v0;
		v1 = f.v1;
		v2 = f.v2;
		normal = f.normal;
		return *this;
	}
}