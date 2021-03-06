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

		inline const Vector3& GetMinimum() const { return m_minimum; }
		inline const Vector3& GetMaximum() const { return m_maximum; }
		inline const float GetSize() const { return m_size; }

		inline bool GetHit(const Ray& r, float tMin, float tMax) const
		{
			if (m_size > 0.f)
			{
				for (uint a = 0; a < 3; ++a)
				{
					const float direction = *((&r.direction.x) + a);
					const float origin = *((&r.origin.x) + a);
					const float min = *((&m_minimum.x) + a);
					const float max = *((&m_maximum.x) + a);

					const float invD = 1.0f / direction;
					float t0 = (min - origin) * invD;
					float t1 = (max - origin) * invD;

					if (invD < 0.0f)
					{
						const float temp = t0;
						t0 = t1;
						t1 = temp;
					}

					tMin = t0 > tMin ? t0 : tMin;
					tMax = t1 < tMax ? t1 : tMax;

					if (tMax < tMin)
					{
						return false;
					}
				}

				return true;
			}
			else if (m_size == 0.f) 
			{
				return false;
			}

			return true; // Special case for always hittable boxes
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