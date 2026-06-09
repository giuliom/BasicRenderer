#pragma once

#include <vector>
#include "Global.h"
#include "MeshInstance.h"

namespace BasicRenderer
{
	class BoundingVolumeHierarchy;

	using AccelerationStructure = BoundingVolumeHierarchy;
	using InstanceList = std::vector<std::shared_ptr<MeshInstance>>;

	class BoundingVolumeHierarchy
	{
	public:

		// Flat, cache-friendly 32-byte node.
		// Children of an internal node are stored contiguously at leftFirst and leftFirst + 1
		struct Node
		{
			Vector3 boundsMin;
			uint32_t leftFirst;	// Internal node: index of the left child. Leaf: index of the first primitive
			Vector3 boundsMax;
			uint32_t primCount;	// Number of primitives in the leaf, 0 for internal nodes

			inline bool IsLeaf() const noexcept { return primCount > 0u; }
		};

	protected:

		std::vector<Node> m_nodes;
		std::vector<const Primitive*> m_primitives;				// Leaf primitives, ordered to match leaf ranges
		std::vector<const Primitive*> m_unboundedPrimitives;	// Primitives without a finite bounding box (e.g. infinite planes), always tested
		uint m_treeLevels = 0u;

		struct BuildEntry;
		void Subdivide(const uint32_t nodeIndex, std::vector<BuildEntry>& entries, const uint32_t start, const uint32_t count, const uint depth);

	public:

		BoundingVolumeHierarchy() noexcept = default;

		uint LevelsCount() const noexcept { return m_treeLevels; }

		const Primitive* GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const;

		void Build(const InstanceList& instances);


		void DebugPrint();

	};

	inline const Primitive* Raycast(const AccelerationStructure& accStruct, const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal)
	{
		HitResult hit;
		const Primitive* anyHit = accStruct.GetHit(r, tMin, tMax, hit);

		if (anyHit != nullptr)
		{
			hitPosition = r.GetPoint(hit.t);
			hitNormal = hit.normal;
		}

		return anyHit;
	}
}