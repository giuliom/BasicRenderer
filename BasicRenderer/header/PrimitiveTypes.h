#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Vertex.h"
#include "Ray.h"
#include "SceneObject.h"

namespace BasicRenderer
{
	class Material;

	//WARNING: World space assumed for all these primitives

	class Sphere : public Primitive
	{
	public:
		const Vector3 m_originalPos;
		Vector3 m_pos;
		const float m_originalRadius;
		float m_radius;

		Sphere(const std::string& name = "") 
			: Primitive({}, nullptr, name), m_originalPos(), m_pos(), m_originalRadius(1.f), m_radius(1.f) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }
		Sphere(const Vector3& pos, float radius, std::shared_ptr<Material> mat = nullptr, const std::string& name = "") 
			: Primitive({}, mat, name), m_originalPos(pos), m_pos(pos), m_originalRadius(radius), m_radius(radius) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }
		~Sphere() {}

		void ProcessForRendering(const Transform& transform) override;
		Primitive* CloneForRendering() const override;
		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const override;

		inline bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const override
		{
			const Vector3 oc = r.origin - m_pos;
			const float a = Vector3::Dot(r.direction, r.direction);
			const float b = Vector3::Dot(oc, r.direction);
			const float c = Vector3::Dot(oc, oc) - m_radius * m_radius;
			const float discriminant = b * b - a * c;
			const float sqDiscr = sqrtf(discriminant);

			if (discriminant > 0.f)
			{
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



	class Plane : public Primitive
	{
	public:
		Vector3 m_centre, m_normal;

		Plane() = delete;
		Plane(const Vector3& centre, const Vector3& normal, std::shared_ptr<Material> mat = nullptr, const std::string& name = "") 
			: Primitive({}, mat, name), m_centre(centre), m_normal(normal.Normalize()) { m_boundingBox = UpdateAxisAlignedBoundingBox(); }
		~Plane() {}

		void ProcessForRendering(const Transform& transform) override;
		Primitive* CloneForRendering() const override;
		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const override;

		inline bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const override
		{
			tMin; 
			tMax;
			const float div = Vector3::Dot(m_normal, r.direction);

			if (abs(div) > 0.0001f)
			{
				float t = Vector3::Dot(m_centre - r.origin, m_normal) / div;

				if (t >= 0.0001f)
				{
					outHit.t = t;
					outHit.normal = m_normal;
					return true;
				}
			}

			return false;
		}

	};
}