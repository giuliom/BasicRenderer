#include "AccelerationStructure.h"
#include <algorithm>
#include <limits>
#include <iostream>
#include "Material.h"

namespace BasicRenderer
{
	namespace
	{
		// Branchless slab test against the current closest hit (outHit.t as tMax).
		// Returns the entry distance, or FLT_MAX if there is no hit.
		inline float IntersectAABB(const Vector3& boundsMin, const Vector3& boundsMax, const Ray& r, const float tMin, const float tMax) noexcept
		{
			const Vector3& o = r.GetOrigin();
			const Vector3& invD = r.GetInvDirection();

			const float tx1 = (boundsMin.x - o.x) * invD.x;
			const float tx2 = (boundsMax.x - o.x) * invD.x;
			float tNear = std::min(tx1, tx2);
			float tFar = std::max(tx1, tx2);

			const float ty1 = (boundsMin.y - o.y) * invD.y;
			const float ty2 = (boundsMax.y - o.y) * invD.y;
			tNear = std::max(tNear, std::min(ty1, ty2));
			tFar = std::min(tFar, std::max(ty1, ty2));

			const float tz1 = (boundsMin.z - o.z) * invD.z;
			const float tz2 = (boundsMax.z - o.z) * invD.z;
			tNear = std::max(tNear, std::min(tz1, tz2));
			tFar = std::min(tFar, std::max(tz1, tz2));

			if (tFar >= tNear && tNear < tMax && tFar > tMin)
			{
				return tNear;
			}
			return std::numeric_limits<float>::max();
		}

		constexpr uint32_t MaxLeafSize = 2u;
		constexpr uint32_t MaxTraversalDepth = 64u;
	}

	struct BoundingVolumeHierarchy::BuildEntry
	{
		PrimitiveRef primitive;
		Vector3 boundsMin;
		Vector3 boundsMax;
		Vector3 centroid;
	};

	bool BoundingVolumeHierarchy::GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const
	{
		uint32_t closestInstanceIndex = 0u;
		bool anyHit = false;
		outHit.t = tMax;

		if (m_nodes.empty())
		{
			return false;
		}

		const Node* const nodes = m_nodes.data();
		const PrimitiveRef* const prims = m_primitives.data();
		const Face* const faces = m_faces;
		const DrawableInstance* const instances = m_instances;

		const auto intersect = [&](const PrimitiveRef& prim) noexcept
		{
			if (prim.IsAnalytic())
			{
				return instances[prim.GeometryIndex()].GetHit(r, tMin, outHit.t, outHit);
			}
			return faces[prim.GeometryIndex()].GetHit(r, tMin, outHit.t, outHit);
		};

		uint32_t stack[MaxTraversalDepth];
		uint32_t stackSize = 0u;
		uint32_t nodeIndex = 0u;

		if (IntersectAABB(nodes[0].boundsMin, nodes[0].boundsMax, r, tMin, outHit.t) != std::numeric_limits<float>::max())
		{
			while (true)
			{
				const Node& node = nodes[nodeIndex];

				if (node.IsLeaf())
				{
					const uint32_t end = node.leftFirst + node.primCount;
					for (uint32_t i = node.leftFirst; i < end; i++)
					{
						if (intersect(prims[i])) // t updated in the call
						{
							closestInstanceIndex = prims[i].instanceIndex;
							anyHit = true;
						}
					}

					if (stackSize == 0u)
					{
						break;
					}
					nodeIndex = stack[--stackSize];
					continue;
				}

				// Internal node: test both children, traverse the nearest first
				const uint32_t leftIndex = node.leftFirst;
				const uint32_t rightIndex = leftIndex + 1u;

				float distLeft = IntersectAABB(nodes[leftIndex].boundsMin, nodes[leftIndex].boundsMax, r, tMin, outHit.t);
				float distRight = IntersectAABB(nodes[rightIndex].boundsMin, nodes[rightIndex].boundsMax, r, tMin, outHit.t);

				uint32_t nearIndex = leftIndex;
				uint32_t farIndex = rightIndex;
				if (distRight < distLeft)
				{
					std::swap(distLeft, distRight);
					nearIndex = rightIndex;
					farIndex = leftIndex;
				}

				if (distLeft == std::numeric_limits<float>::max()) // No child hit
				{
					if (stackSize == 0u)
					{
						break;
					}
					nodeIndex = stack[--stackSize];
				}
				else
				{
					nodeIndex = nearIndex;
					if (distRight != std::numeric_limits<float>::max())
					{
						stack[stackSize++] = farIndex;
					}
				}
			}
		}

		if (anyHit)
		{
			outHit.material = instances[closestInstanceIndex].GetMaterial();
			return true;
		}
		return false;
	}

	void BoundingVolumeHierarchy::Subdivide(const uint32_t nodeIndex, std::vector<BuildEntry>& entries, const uint32_t start, const uint32_t count, const uint depth)
	{
		Node& node = m_nodes[nodeIndex];

		// Compute the node bounds
		Vector3 boundsMin = entries[start].boundsMin;
		Vector3 boundsMax = entries[start].boundsMax;
		for (uint32_t i = start + 1u; i < start + count; i++)
		{
			boundsMin = Vector3::Min(boundsMin, entries[i].boundsMin);
			boundsMax = Vector3::Max(boundsMax, entries[i].boundsMax);
		}
		node.boundsMin = boundsMin;
		node.boundsMax = boundsMax;

		m_treeLevels = std::max(m_treeLevels, depth + 1u);

		if (count <= MaxLeafSize || depth + 2u >= MaxTraversalDepth)
		{
			node.leftFirst = static_cast<uint32_t>(m_primitives.size());
			node.primCount = count;
			for (uint32_t i = start; i < start + count; i++)
			{
				m_primitives.push_back(entries[i].primitive);
			}
			return;
		}

		// Split along the longest axis at the median centroid
		const Vector3 extent = boundsMax - boundsMin;
		int axis = 0;
		if (extent.y > extent.x) { axis = 1; }
		if (extent.z > (axis == 0 ? extent.x : extent.y)) { axis = 2; }

		const auto centroidOf = [axis](const BuildEntry& e) noexcept
		{
			return axis == 0 ? e.centroid.x : (axis == 1 ? e.centroid.y : e.centroid.z);
		};

		const uint32_t mid = start + count / 2u;
		std::nth_element(entries.begin() + start, entries.begin() + mid, entries.begin() + start + count,
			[&centroidOf](const BuildEntry& a, const BuildEntry& b) { return centroidOf(a) < centroidOf(b); });

		const uint32_t leftIndex = static_cast<uint32_t>(m_nodes.size());
		node.leftFirst = leftIndex;
		node.primCount = 0u;

		m_nodes.emplace_back();
		m_nodes.emplace_back();

		Subdivide(leftIndex, entries, start, mid - start, depth + 1u);
		Subdivide(leftIndex + 1u, entries, mid, start + count - mid, depth + 1u);
	}

	void BoundingVolumeHierarchy::Build(const DrawableInstanceList& instances, const FaceBuffer& faceBuffer)
	{
		m_nodes.clear();
		m_primitives.clear();
		m_faces = faceBuffer.data();
		m_instances = instances.data();
		m_treeLevels = 0u;

		std::vector<BuildEntry> entries;
		entries.reserve(faceBuffer.size() + instances.size());

		const auto addPrimitive = [&entries](const PrimitiveRef prim, const AxisAlignedBoundingBox& box)
		{
			if (box.GetSize() <= 0.f)
			{
				// Degenerate primitives are skipped
				return;
			}

			entries.push_back({ prim, box.GetMinimum(), box.GetMaximum(), (box.GetMinimum() + box.GetMaximum()) * 0.5f });
		};

		for (size_t i = 0u; i < instances.size(); ++i)
		{
			const DrawableInstance& instance = instances[i];
			const uint32_t instanceIndex = static_cast<uint32_t>(i);

			if (instance.IsMesh())
			{
				const uint32_t firstFace = instance.GetFirstFace();
				const uint32_t faceCount = instance.NumFaces();

				for (uint32_t faceOffset = 0u; faceOffset < faceCount; ++faceOffset)
				{
					const uint32_t faceIndex = firstFace + faceOffset;
					const Face& face = faceBuffer[faceIndex];
					addPrimitive(PrimitiveRef::FromFace(faceIndex, instanceIndex), face.UpdateAxisAlignedBoundingBox());
				}
			}
			else
			{
				addPrimitive(PrimitiveRef::FromAnalytic(instanceIndex), instance.GetAxisAlignedBoundingBox());
			}
		}

		if (entries.empty())
		{
			return;
		}

		m_nodes.reserve(entries.size() * 2u);
		m_primitives.reserve(entries.size());

		m_nodes.emplace_back(); // root
		Subdivide(0u, entries, 0u, static_cast<uint32_t>(entries.size()), 0u);
	}

	void BoundingVolumeHierarchy::DebugPrint()
	{
		std::cout << std::endl << "BVH: " << m_nodes.size() << " nodes, " << m_primitives.size() << " primitives, "
			<< m_treeLevels << " levels" << std::endl;

		std::vector<uint32_t> currentLevel;

		if (!m_nodes.empty())
		{
			currentLevel.push_back(0u);
		}

		while (!currentLevel.empty())
		{
			std::cout << std::endl;

			std::vector<uint32_t> followingLevel;

			for (const uint32_t index : currentLevel)
			{
				const Node& n = m_nodes[index];
				if (n.IsLeaf())
				{
					std::cout << "[" << n.primCount << "]    ";
				}
				else
				{
					std::cout << "o    ";
					followingLevel.push_back(n.leftFirst);
					followingLevel.push_back(n.leftFirst + 1u);
				}
			}

			currentLevel = std::move(followingLevel);
		}

		std::cout << std::endl;
	}

}
