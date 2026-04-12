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

		Vertex() noexcept : pos(Position::Zero()), nrml(Normal::Zero()), uv(UV::Zero()) {}
		Vertex(const Position& pos_, const Normal& nrml_, const UV& uv_) noexcept : pos(pos_), nrml(nrml_), uv(uv_) {}
		Vertex(const Vector3& pos_v3, const Vector3& nrml_v3, const UV& uv_) noexcept : pos(pos_v3.x, pos_v3.y, pos_v3.z, 1.f), nrml(nrml_v3.x, nrml_v3.y, nrml_v3.z, 1.f), uv(uv_) {}
		Vertex(const Vertex&) noexcept = default;
		Vertex(Vertex&&) noexcept = default;
		~Vertex() noexcept = default;

		Vertex& operator=(const Vertex&) noexcept = default;
		Vertex& operator=(Vertex&&) noexcept = default;
		bool operator==(const Vertex& v) const noexcept;
		bool operator!=(const Vertex& v) const noexcept;

		inline Vertex GetMatrixVertex(const Matrix4& m) const noexcept { return Vertex(m * pos, nrml, uv); }
	};
}
