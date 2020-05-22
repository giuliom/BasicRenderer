#pragma once

#include "Global.h"
#include "Vertex.h"
#include "Primitive.h"

namespace BasicRenderer
{
	class Ray;
	struct HitHit;

	struct Face
	{
	public:
		Vertex v0;
		Vertex v1;
		Vertex v2;
		Vector3 normal;

		Face() {}
		Face(const Vertex& pV0, const Vertex& pV1, const Vertex& pV2) : v0(pV0), v1(pV1), v2(pV2), normal(CalculateNormal(v0.pos, v1.pos, v2.pos)) {}
		Face(const Position& p0, const Position& p1, const Position& p2, const Face& face);
		Face(const Face& f) : v0(f.v0), v1(f.v1), v2(f.v2), normal(f.normal) {}
		Face(Face&& f) : v0(f.v0), v1(f.v1), v2(f.v2), normal(f.normal) {}

		Face& operator=(const Face& f);
		Face& operator=(Face&& f);
	};

	inline Vector3 CalculateNormal(const Face& f) { return CalculateNormal(f.v0.pos, f.v1.pos, f.v2.pos); }

	inline void ToMatrixSpace(Face& f, const Matrix4& m)
	{
		f.v0 = { m * f.v0.pos, m * f.v0.nrml, f.v0.uv };
		f.v1 = { m * f.v1.pos, m * f.v1.nrml, f.v1.uv };
		f.v2 = { m * f.v2.pos, m * f.v2.nrml, f.v2.uv };
		f.normal = CalculateNormal(f);
	}

	//Möller–Trumbore intersection algorithm
	inline bool Intersection(const Face& face, const Ray& r, float tMin, float tMax, float& tHit)
	{
		const Vector3 edge1(face.v1.pos - face.v0.pos);
		const Vector3 edge2(face.v2.pos - face.v0.pos);
		const Vector3 h(Vector3::CrossProduct(r.direction, edge2));
		const float a = Vector3::Dot(edge1, h);

		if (a < EPSILON || std::abs(a) < EPSILON)
		{
			return false; // This ray is parallel to this triangle or facing its back
		}

		const float f = 1.0f / a;
		const Vector3 s(r.origin - face.v0.pos);
		const float u = f * Vector3::Dot(s, h);

		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}

		const Vector3 q(Vector3::CrossProduct(s, edge1));
		const float v = f * Vector3::Dot(r.direction, q);

		if (v < 0.0f || u + v > 1.0f)
		{
			return false;
		}

		// At this stage we can compute t to find out where the intersection point is on the line.
		const float t = f * Vector3::Dot(edge2, q);

		if (t > EPSILON && t >= tMin && t <= tMax) // ray intersection
		{
			tHit = t;
			return true;
		}
		else // This means that there is a line intersection but not a ray intersection.
		{
			return false;
		}
	}
}