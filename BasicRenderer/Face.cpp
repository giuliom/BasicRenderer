#include "Face.h"
#include "Primitive.h"

namespace BasicRenderer
{
	Face::Face(const Position& p0, const Position& p1, const  Position& p2, const Face& f)
		: v0(p0, f.v0.nrml, f.v0.uv)
		, v1(p1, f.v1.nrml, f.v1.uv)
		, v2(p2, f.v2.nrml, f.v2.uv)
		, normal(CalculateNormal(v0.pos, v1.pos, v2.pos))
	{
	}

	//Möller–Trumbore intersection algorithm
	bool Face::GetHit(const Ray& r, float tMin, float tMax, float& tHit) const
	{
		const float EPSILON = 0.0000001f;
		const Vector3 edge1 = v1.pos - v0.pos;
		const Vector3 edge2 = v2.pos - v0.pos;
		const Vector3 h = Vector3::CrossProduct(r.direction, edge2);
		const float a = Vector3::Dot(edge1, h);

		if (a > -EPSILON && a < EPSILON)
		{
			return false;    // This ray is parallel to this triangle.
		}

		const float f = 1.0f / a;
		const Vector3 s = r.origin - v0.pos;
		const float u = f * Vector3::Dot(s, h);

		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}

		const Vector3 q = Vector3::CrossProduct(s, edge1);
		const float v = f * Vector3::Dot(r.direction, q);

		if (v < 0.0f || u + v > 1.0f)
		{
			return false;
		}

		// At this stage we can compute t to find out where the intersection point is on the line.
		const float t = f * Vector3::Dot(edge2, q);

		if (t > EPSILON) // ray intersection
		{
			tHit = t;
			return true;
		}
		else // This means that there is a line intersection but not a ray intersection.
		{
			return false;
		}

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