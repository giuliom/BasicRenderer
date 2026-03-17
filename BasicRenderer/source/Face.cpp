#include "Face.h"

namespace BasicRenderer
{
	Face::Face(const Position& p0, const Position& p1, const  Position& p2, const Face& f)
		: v{{{p0, f.v[0].nrml, f.v[0].uv}, {p1, f.v[1].nrml, f.v[1].uv}, {p2, f.v[2].nrml, f.v[2].uv}}}
		, normal(CalculateNormal(v[0].pos, v[1].pos, v[2].pos))
	{
	}

	Face& Face::operator=(const Face& f)
	{
		v[0] = f.v[0];
		v[1] = f.v[1];
		v[2] = f.v[2];
		normal = f.normal;
		return *this;
	}

	Face& Face::operator=(Face&& f)
	{
		v[0] = f.v[0];
		v[1] = f.v[1];
		v[2] = f.v[2];
		normal = f.normal;
		return *this;
	}
}