#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"

using Position = Vector4;
using Normal = Vector4;
using Color = Vector3;
using UV = Vector2;

const float PI = 3.1415926f;

struct Vertex 
{
public:
	Position pos;
	Normal nrml;
	UV uv;

	Vertex() : pos(Position::Zero()), nrml(Normal::Zero()), uv(UV::Zero()) {}
	Vertex(const Vertex& v) : pos(v.pos), nrml(v.nrml), uv(v.uv) {}
	Vertex(Vertex&& v) : pos(v.pos), nrml(v.nrml), uv(v.uv) {}
	~Vertex() {}

	Vertex(const Position& pos_, const Normal& nrml_, const UV& uv_) : pos(pos_), nrml(nrml_), uv(uv_) {}
	Vertex& operator=(const Vertex& v);
	Vertex& operator=(Vertex&& v);
	
	Vertex GetMatrixVertex(const Matrix4& m) const;
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

	Face ToMatrixSpace(const Matrix4& m) const;

	Vector3 CalculateNormal() const;
};


struct FaceIndices
{
public:
	unsigned int i0;
	unsigned int i1;
	unsigned int i2;

	FaceIndices(unsigned int i0_, unsigned int i1_, unsigned int i2_) : i0(i0_), i1(i1_), i2(i2_) {}
};


struct Sphere
{
public:
	Position pos;
	float radius;

	Sphere(Position pos_, float radius_) : pos(pos_), radius(radius_) {}
};


struct Cube
{
public:
	Position pos;
	float sideSize;

	Cube(Position pos_, float sideSize_) : pos(pos_), sideSize(sideSize_) {}
};
