#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"

using Position = Vector4;
using Normal = Vector4;
using Color = Vector3;
using UV = Vector2;


struct Vertex
{
public:

	Position pos;
	Normal nrml;
	UV uv;

	Vertex() : pos(Position::Zero()), nrml(Normal::Zero()), uv(UV::Zero()) {}
	Vertex(const Position& pos_, const Normal& nrml_, const UV& uv_) : pos(pos_), nrml(nrml_), uv(uv_) {}
	Vertex(const Vertex& v) : pos(v.pos), nrml(v.nrml), uv(v.uv) {}
	Vertex(Vertex&& v) : pos(v.pos), nrml(v.nrml), uv(v.uv) {}
	~Vertex() {}

	Vertex& operator=(const Vertex& v);
	Vertex& operator=(Vertex&& v);

	inline Vertex GetMatrixVertex(const Matrix4& m) const { return Vertex(m * pos, nrml, uv); }
};
