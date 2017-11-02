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

	Vertex() : pos(Position::Zero()), nrml(Normal::Zero()), uv(UV::Zero())
	{}

	Vertex(const Position& pos, const Normal& nmrl, const UV& uv) : pos(pos), nrml(nrml), uv(uv) {}
	Vertex& operator=(const Vertex& v);
	
	Vertex GetMatrixVertex(const Matrix4& m) const;
};


struct Face
{
public:
	Vertex v0;
	Vertex v1;
	Vertex v2;

	Face();
	Face(const Vertex& v0, const Vertex& v1, const Vertex& v2) : v0(v0), v1(v1), v2(v2) {}
	Face(const Position& p0, const Position& p1, const Position& p2, const Face& face);
	Face& operator=(const Face& f);

	Face ToMatrixSpace(const Matrix4& m) const;

	Vector3 CalculateNormal() const;
};


struct FaceIndices
{
public:
	unsigned int i0;
	unsigned int i1;
	unsigned int i2;

	FaceIndices(unsigned int i0, unsigned int i1, unsigned int i2) : i0(i0), i1(i1), i2(i2) {}
};


struct Sphere
{
public:
	Position pos;
	float radius;

	Sphere(Position pos, float radius) : pos(pos), radius(radius) {}
};


struct Cube
{
public:
	Position pos;
	float sideSize;

	Cube(Position pos, float sideSize) : pos(pos), sideSize(sideSize) {}
};
