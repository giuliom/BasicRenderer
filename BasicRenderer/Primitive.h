#pragma once

#include <memory>
#include <algorithm>
#include "Global.h"
#include "Vector3.h"
#include "Ray.h"
#include "Matrix4.h"

namespace BasicRenderer
{
	class Material;
	class AxisAlignedBoundingBox;

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

					if (tMax <= tMin)
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
		inline AxisAlignedBoundingBox operator+(const AxisAlignedBoundingBox& other)
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


	class BVHnode
	{
		AxisAlignedBoundingBox m_box;
		std::unique_ptr<BVHnode> m_left;
		std::unique_ptr<BVHnode> m_right;

	public:

		BVHnode(const AxisAlignedBoundingBox& box, BVHnode* left, BVHnode* right) : m_box(box), m_left(left), m_right(right) {}

		inline bool GetHit(const Ray& r, float tMin, float tMax) const
		{
			if (m_box.GetHit(r, tMin, tMax) == false)
			{
				return false;
			}

			bool hit_left = false;

			if (m_left != nullptr)
			{
				hit_left = m_left->GetHit(r, tMin, tMax);
			}

			bool hit_right = false;

			if (m_right != nullptr)
			{
				hit_right = m_right->GetHit(r, tMin, tMax);
			}

			return hit_left || hit_right;
		}
	};


	class Primitive
	{
	protected:
		//TODO temporary implementation
		static uint m_idCounter;
		uint m_id;
		AxisAlignedBoundingBox m_boundingBox;
		const Material* m_material = nullptr;

		virtual void UpdateAxisAlignedBoundingBox() = 0;

	public:

		Primitive() : m_id(m_idCounter++), m_boundingBox() {}
		Primitive(Material* mat) : m_id(m_idCounter++), m_boundingBox(), m_material(mat) {}
		Primitive(const Primitive& other) : m_id(m_idCounter++), m_boundingBox(other.GetAxisAlignedBoundingBox()), m_material(other.GetMaterial()) {}

		inline uint GetId() const { return m_id; }

		virtual void ProcessForRendering() = 0;
		virtual bool GetHit(const Ray& r, float tMin, float tMax, float& tHit, Vector3& normalHit) const = 0;

		inline const AxisAlignedBoundingBox& GetAxisAlignedBoundingBox() const { return m_boundingBox; }
		inline const Material* GetMaterial() const { return m_material; }
	};
}