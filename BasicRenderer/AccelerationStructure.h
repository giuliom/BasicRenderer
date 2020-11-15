#pragma once

#include "Global.h"
#include "Primitive.h"


namespace BasicRenderer
{
	class BVHnode
	{
		AxisAlignedBoundingBox m_box;
		std::unique_ptr<BVHnode> m_left;
		std::unique_ptr<BVHnode> m_right;

		//TODO link to primitives by id?

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


	class BoundingVolumeHierarchy
	{
	protected:

		BVHnode m_root;

	public:

		BoundingVolumeHierarchy() : m_root(AxisAlignedBoundingBox(), nullptr, nullptr) {}

		inline bool GetHit(const Ray& r, float tMin, float tMax) const
		{
			return m_root.GetHit(r, tMin, tMax);
		}

		void Build();
	};
}