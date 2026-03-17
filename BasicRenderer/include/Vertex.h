#pragma once

#include "Global.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Color.h"

namespace BasicRenderer
{
	using Position = Vector4;
	using Normal = Vector4;
	using UV = Vector2;


	struct Vertex
	{
	public:

		Position pos;
		Normal nrml;
		UV uv;

		Vertex() : pos(Position::Zero()), nrml(Normal::Zero()), uv(UV::Zero()) {}
		Vertex(const Position& pos_, const Normal& nrml_, const UV& uv_) : pos(pos_), nrml(nrml_), uv(uv_) {}
		Vertex(const Vector3& pos_v3, const Vector3& nrml_v3, const UV& uv_) : pos(pos_v3.x, pos_v3.y, pos_v3.z, 1.f), nrml(nrml_v3.x, nrml_v3.y, nrml_v3.z, 1.f), uv(uv_) {}
		Vertex(const Vertex&) = default;
		Vertex(Vertex&&) = default;
		~Vertex() = default;

		Vertex& operator=(const Vertex&) = default;
		Vertex& operator=(Vertex&&) = default;
		bool operator==(const Vertex& v) const;
		bool operator!=(const Vertex& v) const;

		inline Vertex GetMatrixVertex(const Matrix4& m) const { return Vertex(m * pos, nrml, uv); }
	};
}
