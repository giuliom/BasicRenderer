#pragma once

#include <vector>
#include "Global.h"
#include "DrawableInstance.h"

namespace BasicRenderer
{
	class BoundingVolumeHierarchy;

	using AccelerationStructure = BoundingVolumeHierarchy;

	class BoundingVolumeHierarchy
	{
	public:

		// Flat, cache-friendly 32-byte node.
		// Children of an internal node are stored contiguously at leftFirst and leftFirst + 1
		struct alignas(32) Node
		{
			Vector3 boundsMin;
			uint32_t leftFirst;	// Internal node: index of the left child. Leaf: index of the first primitive
			Vector3 boundsMax;
			uint32_t primCount;	// Number of primitives in the leaf, 0 for internal nodes

			inline bool IsLeaf() const noexcept { return primCount > 0u; }
		};
		static_assert(sizeof(Node) == 32u, "BVH nodes must remain 32 bytes");
		static_assert(alignof(Node) == 32u, "BVH nodes must remain 32-byte aligned");

	protected:
		// Compact non-owning reference into the RenderState buffers. The high bit
		// distinguishes analytic instances from faces; instanceIndex owns material.
		struct alignas(8) PrimitiveRef
		{
			static constexpr uint32_t AnalyticMask = uint32_t{ 1 } << 31u;
			static constexpr uint32_t IndexMask = ~AnalyticMask;

			uint32_t indexAndKind;
			uint32_t instanceIndex;

			static PrimitiveRef FromFace(const uint32_t faceIndex, const uint32_t ownerIndex) noexcept
			{
				return { faceIndex, ownerIndex };
			}

			static PrimitiveRef FromAnalytic(const uint32_t ownerIndex) noexcept
			{
				return { ownerIndex | AnalyticMask, ownerIndex };
			}

			bool IsAnalytic() const noexcept { return (indexAndKind & AnalyticMask) != 0u; }
			uint32_t GeometryIndex() const noexcept { return indexAndKind & IndexMask; }
		};
		static_assert(sizeof(PrimitiveRef) == 8u, "BVH primitive references must remain compact");

		std::vector<Node> m_nodes;
		std::vector<PrimitiveRef> m_primitives;	// Leaf primitives, ordered to match leaf ranges
		const Face* m_faces = nullptr;				// Owned by the RenderState that owns this BVH
		const DrawableInstance* m_instances = nullptr;	// Owned by the RenderState that owns this BVH
		uint m_treeLevels = 0u;

		struct BuildEntry;
		void Subdivide(const uint32_t nodeIndex, std::vector<BuildEntry>& entries, const uint32_t start, const uint32_t count, const uint depth);

	public:

		BoundingVolumeHierarchy() noexcept = default;

		uint LevelsCount() const noexcept { return m_treeLevels; }

		bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const;

		void Build(const DrawableInstanceList& instances, const FaceBuffer& faceBuffer);


		void DebugPrint();

	};

	inline bool Raycast(const AccelerationStructure& accStruct, const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal, HitResult& outHit)
	{
		const bool anyHit = accStruct.GetHit(r, tMin, tMax, outHit);

		if (anyHit)
		{
			hitPosition = r.GetPoint(outHit.t);
			hitNormal = outHit.normal;
		}

		return anyHit;
	}
}
