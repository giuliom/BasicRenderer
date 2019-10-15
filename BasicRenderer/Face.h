#pragma once

#include "Vertex.h"

class Ray;
struct HitHit;

struct FaceIndices
{
public:
	unsigned int i0;
	unsigned int i1;
	unsigned int i2;

	FaceIndices(unsigned int i0_, unsigned int i1_, unsigned int i2_) : i0(i0_), i1(i1_), i2(i2_) {}
};

struct Face
{
public:
	Vertex v0;
	Vertex v1;
	Vertex v2;
	Vector3 normal;

	Face() {}
	Face(const Vertex& v0_, const Vertex& v1_, const Vertex& v2_) : v0(v0_), v1(v1_), v2(v2_), normal(CalculateNormal()) {}
	Face(const Position& p0, const Position& p1, const Position& p2, const Face& face);
	Face(const Face& f) : v0(f.v0), v1(f.v1), v2(f.v2), normal(f.normal) {}
	Face(Face&& f) : v0(f.v0), v1(f.v1), v2(f.v2), normal(f.normal) {}

	Face& operator=(const Face& f);
	Face& operator=(Face&& f);

	virtual bool GetHit(const Ray& r, float tMin, float tMax, float& tHit) const;

	inline Face ToMatrixSpace(const Matrix4& m) const
	{
		return Face(Vertex(m * v0.pos, m * v0.nrml, v0.uv),
					Vertex(m * v1.pos, m * v1.nrml, v1.uv),
					Vertex(m * v2.pos, m * v2.nrml, v2.uv));
	}

	inline Vector3 CalculateNormal() const { return Vector3::CrossProduct(v1.pos - v0.pos, v2.pos - v0.pos).Normalize(); }
};

