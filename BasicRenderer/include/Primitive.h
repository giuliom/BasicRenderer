#pragma once

#include <concepts>
#include <memory>
#include <algorithm>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "Global.h"
#include "Vector3.h"
#include "Ray.h"
#include "Matrix4.h"
#include "Transform.h"

namespace BasicRenderer
{
	class Material;
	class Primitive;
	class Face;
	class Sphere;
	class Plane;

	using PrimitiveData = std::variant<Face, Sphere, Plane>;
	using PrimitiveList = std::vector<std::unique_ptr<Primitive>>;

	struct HitResult
	{
		Primitive* primitive;
		Material* material;
		float t;
		Vector3 normal;

		HitResult() noexcept : primitive(nullptr), material(nullptr), t(0.f), normal() {}
	};

	class AxisAlignedBoundingBox
	{
		friend class Plane;

	protected:
		Vector3 m_minimum;
		Vector3 m_maximum;
		float m_size;

	public:

		AxisAlignedBoundingBox() noexcept : m_minimum({ 0.f, 0.f, 0.f }), m_maximum({ 0.f, 0.f, 0.f }), m_size(0.f) {}
		AxisAlignedBoundingBox(const Vector3& minimum, const Vector3& maximum) noexcept : m_minimum(minimum), m_maximum(maximum), m_size((maximum - minimum).Length()) {}
		AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other) noexcept : m_minimum(other.m_minimum), m_maximum(other.m_maximum), m_size(other.m_size) {}
		AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other) noexcept { m_minimum = other.m_minimum; m_maximum = other.m_maximum; m_size = other.m_size; return *this; }

		inline const Vector3& GetMinimum() const noexcept { return m_minimum; }
		inline const Vector3& GetMaximum() const noexcept { return m_maximum; }
		inline float GetSize() const noexcept { return m_size; }

		inline bool GetHit(const Ray& r, const float tMin, const float tMax) const noexcept
		{
			if (m_size <= 0.f)
			{
				// Empty boxes are non-hittable but negative sized ones are
				return m_size < 0.f;
			}

			// X axis
			{
				float t0 = (m_minimum.x - r.GetOrigin().x) * r.GetInvDirection().x;
				float t1 = (m_maximum.x - r.GetOrigin().x) * r.GetInvDirection().x;
				if (r.GetInvDirection().x < 0.0f) { const float temp = t0; t0 = t1; t1 = temp; }
				float localtMin = t0 > tMin ? t0 : tMin;
				float localtMax = t1 < tMax ? t1 : tMax;
				if (localtMax < localtMin) return false;
			}

			// Y axis
			{
				float t0 = (m_minimum.y - r.GetOrigin().y) * r.GetInvDirection().y;
				float t1 = (m_maximum.y - r.GetOrigin().y) * r.GetInvDirection().y;
				if (r.GetInvDirection().y < 0.0f) { const float temp = t0; t0 = t1; t1 = temp; }
				float localtMin = t0 > tMin ? t0 : tMin;
				float localtMax = t1 < tMax ? t1 : tMax;
				if (localtMax < localtMin) return false;
			}

			// Z axis
			{
				float t0 = (m_minimum.z - r.GetOrigin().z) * r.GetInvDirection().z;
				float t1 = (m_maximum.z - r.GetOrigin().z) * r.GetInvDirection().z;
				if (r.GetInvDirection().z < 0.0f) { const float temp = t0; t0 = t1; t1 = temp; }
				float localtMin = t0 > tMin ? t0 : tMin;
				float localtMax = t1 < tMax ? t1 : tMax;
				if (localtMax < localtMin) return false;
			}

			return true;
		}

		// Return the bounding box of the two boxes combined
		inline AxisAlignedBoundingBox operator+(const AxisAlignedBoundingBox& other) const noexcept
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

	enum class PrimitiveType : uint
	{
		SPHERE,
		PLANE,
		FACE
	};

	class Primitive
	{
	protected:
		Material* m_material;
		AxisAlignedBoundingBox m_boundingBox;

	public:
		virtual PrimitiveType GetType() const = 0;
		virtual AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const = 0;

		Primitive() noexcept : m_material(nullptr), m_boundingBox() {}
		Primitive(Material* material) noexcept : m_material(material), m_boundingBox() {}
		Primitive(const Primitive& other) noexcept
			: m_material(other.m_material), m_boundingBox(other.m_boundingBox) {}
		virtual ~Primitive() {}

		void SetMaterial(Material* material) noexcept { m_material = material; }
		inline Material* GetMaterial() const noexcept { return m_material; }

		virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const = 0;
		inline const AxisAlignedBoundingBox& GetAxisAlignedBoundingBox()	const noexcept	{ return m_boundingBox; }
	};

	template<typename T>
		requires std::derived_from<T, Primitive>
	T ProcessForRendering(const T& prim, const Transform& transform);
}