#pragma once

#include "Vertex.h"

class Ray;
struct HitResult;

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

	Face() {}
	Face(const Vertex& v0_, const Vertex& v1_, const Vertex& v2_) : v0(v0_), v1(v1_), v2(v2_) {}
	Face(const Position& p0, const Position& p1, const Position& p2, const Face& face);
	Face(const Face& f) : v0(f.v0), v1(f.v1), v2(f.v2) {}
	Face(Face&& f) : v0(f.v0), v1(f.v1), v2(f.v2) {}

	Face& operator=(const Face& f);
	Face& operator=(Face&& f);

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const;

	inline Face ToMatrixSpace(const Matrix4& m) const
	{
		return Face(Vertex(m * v0.pos, v0.nrml, v0.uv),
					Vertex(m * v1.pos, v1.nrml, v1.uv),
					Vertex(m * v2.pos, v2.nrml, v2.uv));
	}

	Vector3 CalculateNormal() const;
};

