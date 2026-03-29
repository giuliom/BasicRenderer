#include "Face.h"

namespace BasicRenderer
{
	Face::Face(const Position& p0, const Position& p1, const  Position& p2, const Face& f)
		: Primitive(f.GetMaterial())
		,v{{{p0, f.v[0].nrml, f.v[0].uv}, {p1, f.v[1].nrml, f.v[1].uv}, {p2, f.v[2].nrml, f.v[2].uv}}}
		, normal(CalculateNormal(v[0].pos, v[1].pos, v[2].pos))
	{
	}

	AxisAlignedBoundingBox Face::UpdateAxisAlignedBoundingBox() const
	{
		Vector3 min = v[0].pos;
		Vector3 max = v[0].pos;

		for (uint i = 1; i < 3; ++i)
		{
			if (v[i].pos.x < min.x) min.x = v[i].pos.x;
			if (v[i].pos.y < min.y) min.y = v[i].pos.y;
			if (v[i].pos.z < min.z) min.z = v[i].pos.z;
			if (v[i].pos.x > max.x) max.x = v[i].pos.x;
			if (v[i].pos.y > max.y) max.y = v[i].pos.y;
			if (v[i].pos.z > max.z) max.z = v[i].pos.z;
		}

		return AxisAlignedBoundingBox(min, max);
	}

	Face& Face::operator=(const Face& f)
	{
		v[0] = f.v[0];
		v[1] = f.v[1];
		v[2] = f.v[2];
		normal = f.normal;
		m_boundingBox = f.GetAxisAlignedBoundingBox();
		return *this;
	}

	Face& Face::operator=(Face&& f)
	{
		v[0] = f.v[0];
		v[1] = f.v[1];
		v[2] = f.v[2];
		normal = f.normal;
		m_boundingBox = f.GetAxisAlignedBoundingBox();
		return *this;
	}
}