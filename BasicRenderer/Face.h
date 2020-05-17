#pragma once

#include "Global.h"
#include "Vertex.h"

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


	bool GetHit(const Ray& r, float tMin, float tMax, float& tHit) const;
};

inline Vector3 CalculateNormal(const Face& f) { return CalculateNormal(f.v0.pos, f.v1.pos, f.v2.pos); }

inline void ToMatrixSpace(Face& f, const Matrix4& m)
{
	f.v0 = { m * f.v0.pos, m * f.v0.nrml, f.v0.uv };
	f.v1 = { m * f.v1.pos, m * f.v1.nrml, f.v1.uv };
	f.v2 = { m * f.v2.pos, m * f.v2.nrml, f.v2.uv };
	f.normal = CalculateNormal(f);
}