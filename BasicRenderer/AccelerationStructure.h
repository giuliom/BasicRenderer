#pragma once

#include <vector>
#include <unordered_map>
#include "Global.h"
#include "Primitive.h"


namespace BasicRenderer
{
	typedef std::unordered_map<uint, std::unique_ptr<Primitive>> ObjectList;

	class BVHnode
	{
		const Primitive* m_primitive;
		const AxisAlignedBoundingBox m_box;
		std::unique_ptr<const BVHnode> m_left;
		std::unique_ptr<const BVHnode> m_right;

	public:

		BVHnode(const Primitive* primitive, const AxisAlignedBoundingBox& box, const BVHnode* left, const BVHnode* right) : m_primitive(primitive), m_box(box), m_left(left), m_right(right) {}

		inline const Primitive* GetPrimitive() const { return m_primitive; }
		inline const AxisAlignedBoundingBox GetAxisAlignedBoundingBox() const { return m_box; }
		inline const BVHnode* GetLeft() const { return m_left.get(); }
		inline const BVHnode* GetRight() const { return m_right.get(); }

		inline bool GetHit(const Ray& r, float tMin, float tMax) const { return m_box.GetHit(r, tMin, tMax); }
	};


	class BoundingVolumeHierarchy
	{
	protected:

		std::unique_ptr<const BVHnode> m_root;
		uint m_treeLevels;

	public:

		BoundingVolumeHierarchy() : m_root(nullptr), m_treeLevels(0u) {}

		uint LevelsCount() const { return m_treeLevels; }

		const Primitive* GetHit(const Ray& r, float tMin, float tMax, std::vector<const BVHnode*>& dfsStack, HitResult& outHit) const;

		void Build(const ObjectList::iterator& begin_it, const ObjectList::iterator& end_it);


		void DebugPrint();

	};
}