#pragma once

#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include "Global.h"
#include "Vector3.h"
#include "Ray.h"
#include "Matrix4.h"
#include "Transform.h"
#include "Face.h"

namespace BasicRenderer
{
	class Material;
	class Primitive;
	struct Face;

	struct HitResult
	{
		const Primitive* primitive;
		float t;
		Vector3 normal;

		HitResult() : primitive(nullptr), t(0.f), normal() {}
	};

	class AxisAlignedBoundingBox
	{
		friend class Plane;

	protected:

		Vector3 m_minimum;
		Vector3 m_maximum;
		float m_size;

	public:

		AxisAlignedBoundingBox() : m_minimum({ 0.f, 0.f, 0.f }), m_maximum({ 0.f, 0.f, 0.f }), m_size(0.f) {}
		AxisAlignedBoundingBox(const Vector3& minimum, const Vector3& maximum) : m_minimum(minimum), m_maximum(maximum), m_size((maximum - minimum).Length()) {}
		AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other) : m_minimum(other.m_minimum), m_maximum(other.m_maximum), m_size(other.m_size) {}
		AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other) { m_minimum = other.m_minimum; m_maximum = other.m_maximum; m_size = other.m_size; return *this; }

		inline const Vector3& GetMinimum() const { return m_minimum; }
		inline const Vector3& GetMaximum() const { return m_maximum; }
		inline float GetSize() const { return m_size; }

		inline bool GetHit(const Ray& r, const float tMin, const float tMax) const
		{
			if (m_size <= 0.f)
			{
				// Empty boxes are non-hittable but negative sized ones are
				return m_size < 0.f;
			}

			// X axis
			{
				float t0 = (m_minimum.x - r.origin.x) * r.invDirection.x;
				float t1 = (m_maximum.x - r.origin.x) * r.invDirection.x;
				if (r.invDirection.x < 0.0f) { const float temp = t0; t0 = t1; t1 = temp; }
				float localtMin = t0 > tMin ? t0 : tMin;
				float localtMax = t1 < tMax ? t1 : tMax;
				if (localtMax < localtMin) return false;
			}

			// Y axis
			{
				float t0 = (m_minimum.y - r.origin.y) * r.invDirection.y;
				float t1 = (m_maximum.y - r.origin.y) * r.invDirection.y;
				if (r.invDirection.y < 0.0f) { const float temp = t0; t0 = t1; t1 = temp; }
				float localtMin = t0 > tMin ? t0 : tMin;
				float localtMax = t1 < tMax ? t1 : tMax;
				if (localtMax < localtMin) return false;
			}

			// Z axis
			{
				float t0 = (m_minimum.z - r.origin.z) * r.invDirection.z;
				float t1 = (m_maximum.z - r.origin.z) * r.invDirection.z;
				if (r.invDirection.z < 0.0f) { const float temp = t0; t0 = t1; t1 = temp; }
				float localtMin = t0 > tMin ? t0 : tMin;
				float localtMax = t1 < tMax ? t1 : tMax;
				if (localtMax < localtMin) return false;
			}

			return true;
		}

		// Return the bounding box of the two boxes combined
		inline AxisAlignedBoundingBox operator+(const AxisAlignedBoundingBox& other) const
		{
			Vector3 min(std::min(m_minimum.x, other.m_minimum.x),
						std::min(m_minimum.y, other.m_minimum.y),
						std::min(m_minimum.z, other.m_minimum.z));

			Vector3 max(std::max(m_maximum.x, other.m_maximum.x),
						std::max(m_maximum.y, other.m_maximum.y),
						std::max(m_maximum.z, other.m_maximum.z));

			return AxisAlignedBoundingBox(min, max);
		}
	};

	class Primitive
	{
	protected:

		std::string m_name;
		std::shared_ptr<Material> m_material; //TODO support material instances
		std::vector<Face> m_faces;
		AxisAlignedBoundingBox m_boundingBox;

		virtual AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const = 0;

	public:
		Primitive(const std::vector<Face>& faces, std::shared_ptr<Material> mat = nullptr, const std::string& name = "") 
			: m_name(name), m_material(mat), m_faces(faces), m_boundingBox() {}
		Primitive(const Primitive& other) 
			: m_name(other.m_name), m_material(other.m_material), m_faces(other.m_faces), m_boundingBox(other.m_boundingBox) {}
		virtual ~Primitive() {}

		Primitive& operator=(const Primitive& other) = delete;

		virtual Primitive* CloneForRendering() const = 0;
		virtual void ProcessForRendering(const Transform& transform) = 0;
		virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const = 0;

		inline const std::string& GetName()									const	{ return m_name; }
		inline const std::vector<Face>& GetFaces()							const	{ return m_faces; }
		inline const Face& GetFace(uint index)								const	{ return m_faces[index]; }
		inline size_t NumFaces()											const	{ return m_faces.size(); }
		inline const AxisAlignedBoundingBox& GetAxisAlignedBoundingBox()	const	{ return m_boundingBox; }
		inline const Material* GetMaterial()								const	{ return m_material.get(); }
	};
}