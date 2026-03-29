#pragma once

#include <array>
#include "Global.h"
#include "Vertex.h"
#include "Ray.h"
#include "Primitive.h"

namespace BasicRenderer
{
	class Face : public Primitive
	{
	public:
		std::array<Vertex, 3> v;
		Vector3 normal;

		Face() : Primitive(nullptr) {}
		Face(const Vertex& pV0, const Vertex& pV1, const Vertex& pV2, Material* material = nullptr) : Primitive(material), v{pV0, pV1, pV2}, normal(CalculateNormal(v[0].pos, v[1].pos, v[2].pos)) {}
		Face(const Position& p0, const Position& p1, const Position& p2, const Face& face);
		Face(const Face& f) : Primitive(f), v{f.v[0], f.v[1], f.v[2]}, normal(f.normal) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }
		Face(Face&& f) noexcept : Primitive(f), v{f.v[0], f.v[1], f.v[2]}, normal(f.normal) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }

		Face& operator=(const Face& f);
		Face& operator=(Face&& f);

		const Vector3& GetNormal() const { return normal; }
		PrimitiveType GetType() const override { return PrimitiveType::FACE; }
		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const override;
		inline bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const override;

		friend void ToMatrixSpace(Face& f, const Matrix4& m);
	};

	inline Vector3 CalculateNormal(const Face& f) { return CalculateNormal(f.v[0].pos, f.v[1].pos, f.v[2].pos); }

	inline void ToMatrixSpace(Face& f, const Matrix4& m)
	{
		f.v[0] = { m * f.v[0].pos, m * f.v[0].nrml, f.v[0].uv };
		f.v[1] = { m * f.v[1].pos, m * f.v[1].nrml, f.v[1].uv };
		f.v[2] = { m * f.v[2].pos, m * f.v[2].nrml, f.v[2].uv };
		f.normal = CalculateNormal(f);
		f.m_boundingBox = f.UpdateAxisAlignedBoundingBox();
	}

	//Moller-Trumbore intersection algorithm
	inline bool Intersection(const Face& face, const Ray& r, float tMin, float tMax, float& tHit)
	{
		const Vector3 edge1(face.v[1].pos - face.v[0].pos);
		const Vector3 edge2(face.v[2].pos - face.v[0].pos);
		const Vector3 h(Vector3::CrossProduct(r.GetDirection(), edge2));
		const float a = Vector3::Dot(edge1, h);

		if (a < EPSILON || std::abs(a) < EPSILON)
		{
			return false; // This ray is parallel to this triangle or facing its back
		}

		const float f = 1.0f / a;
		const Vector3 s(r.GetOrigin() - face.v[0].pos);
		const float u = f * Vector3::Dot(s, h);

		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}

		const Vector3 q(Vector3::CrossProduct(s, edge1));
		const float v = f * Vector3::Dot(r.GetDirection(), q);

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

	inline bool Face::GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const
	{
		if (Intersection(*this, r, tMin, tMax, outHit.t))
		{
			outHit.normal = normal;
			return true;
		}
		return false;
	}

	inline Face ProcessForRendering(Face face, const Transform& transform)
	{
		ToMatrixSpace(face, transform.GetWorldMatrix());
		return face;
	}
}