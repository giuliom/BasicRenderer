#pragma once

#include <cmath>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Transform.h"
#include "Ray.h"
#include "AABB.h"

namespace BasicRenderer
{
	//WARNING: World space assumed for all these primitives

	class Sphere
	{
	protected:
		AxisAlignedBoundingBox m_boundingBox;

	public:
		Vector3 m_pos;
		float m_radius;

		Sphere() : m_pos(), m_radius(1.f) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }
		Sphere(const Vector3& pos, const float radius = 1.f)
			: m_pos(pos), m_radius(radius) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }
		Sphere(const Sphere& other) noexcept = default;
		Sphere(Sphere&& other) noexcept = default;

		Sphere& operator=(const Sphere& other) noexcept = default;
		Sphere& operator=(Sphere&& other) noexcept = default;

		inline const AxisAlignedBoundingBox& GetAxisAlignedBoundingBox() const noexcept { return m_boundingBox; }

		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const noexcept;

		inline bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const noexcept
		{
			const Vector3 oc = r.GetOrigin() - m_pos;
			const float a = Vector3::Dot(r.GetDirection(), r.GetDirection());
			const float b = Vector3::Dot(oc, r.GetDirection());
			const float c = Vector3::Dot(oc, oc) - m_radius * m_radius;
			const float discriminant = b * b - a * c;

			if (discriminant > 0.f)
			{
				const float sqDiscr = std::sqrt(discriminant);
				float temp = (-b - sqDiscr) / a;
				if (temp > tMin && temp < tMax)
				{
					outHit.t = temp;
					outHit.normal = ((r.GetPoint(temp) - m_pos) / m_radius);
					return true;
				}
				temp = (-b + sqDiscr) / a;
				if (temp > tMin && temp < tMax)
				{
					outHit.t = temp;
					outHit.normal = ((r.GetPoint(temp) - m_pos) / m_radius);
					return true;
				}
			}
			return false;
		}
	};

	inline Sphere ProcessForRendering(const Sphere& original, const Transform& transform)
	{
		const auto pos = original.m_pos + transform.GetWorldPosition();
		Matrix4 wm = transform.GetWorldMatrix();
		const float worldScaleX = Vector3(wm.x1, wm.y1, wm.z1).Length();
		const auto radius = original.m_radius * worldScaleX;
		return {pos, radius};
	}

	class Plane
	{
	protected:
		AxisAlignedBoundingBox m_boundingBox;

	public:
		// Half-extent of the finite bounding box approximating the infinite plane
		static constexpr float MAX_EXTENT = 1000000.f;

		Vector3 m_centre, m_normal;

		Plane() = delete;
		Plane(const Vector3& centre, const Vector3& normal)
			: m_centre(centre), m_normal(normal.Normalize()) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }
		Plane(const Plane& other) noexcept = default;
		Plane(Plane&& other) noexcept = default;

		Plane& operator=(const Plane& other) noexcept = default;
		Plane& operator=(Plane&& other) noexcept = default;

		inline const AxisAlignedBoundingBox& GetAxisAlignedBoundingBox() const noexcept { return m_boundingBox; }

		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const noexcept;

		inline bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const noexcept
		{
			const float div = Vector3::Dot(m_normal, r.GetDirection());

			if (abs(div) > 0.0001f)
			{
				float t = Vector3::Dot(m_centre - r.GetOrigin(), m_normal) / div;

				if (t >= tMin && t <= tMax)
				{
					outHit.t = t;
					outHit.normal = m_normal;
					return true;
				}
			}

			return false;
		}
	};

	inline Plane ProcessForRendering(const Plane& original, const Matrix4& wm)
	{
		const auto centre = wm * original.m_centre;
		const Vector4 n = wm * Vector4(original.m_normal.x, original.m_normal.y, original.m_normal.z, 0.0f);
		const auto normal = Vector3(n.x, n.y, n.z).Normalize();
		return {centre, normal};
	}
}