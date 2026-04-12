#include "AccelerationStructure.h"
#include <algorithm>
#include <iostream>

namespace BasicRenderer
{
	const Primitive* BoundingVolumeHierarchy::GetHit(const Ray& r, float tMin, float tMax, std::vector<const BVHnode*>& dfsStack, HitResult& outHit) const
	{
		const Primitive* tempPrim = nullptr;
		outHit.t = tMax;

		dfsStack.emplace_back(m_root.get());

		while (dfsStack.size() > 0)
		{
			const BVHnode* n = dfsStack.back();
			dfsStack.pop_back();

			if (n != nullptr && n->GetHit(r, tMin, outHit.t)) // AABB is hit
			{
				const Primitive* prim = n->GetPrimitive();

				if (prim != nullptr) // Leaf node
				{
					assert(n->GetLeft() == nullptr && n->GetRight() == nullptr); // must be a leaf node

					if (prim->GetHit(r, tMin, outHit.t, outHit)) // Primitive hit, t updated in the call
					{
						tempPrim = prim;
					}
				}
				else // Non leaf node, adding children to the stack
				{
					dfsStack.emplace_back(n->GetRight());
					dfsStack.emplace_back(n->GetLeft());
				}
			}
		}

		return tempPrim;
	}

	void BoundingVolumeHierarchy::Build(const InstanceList& instances)
	{
		if (instances.size() == 0)
		{
			return;
		}

		// Building the nodes bottom-up
		std::vector<std::unique_ptr<const BVHnode>> nodes;
		nodes.reserve(instances.size());

		for (const auto& instance : instances)
		{
			for (size_t i = 0; i < instance->NumPrimitives(); i++)
			{
				const auto* prim = instance->GetPrimitive(i);
				if (prim)
				{
					nodes.emplace_back(std::make_unique<BVHnode>(prim, prim->GetAxisAlignedBoundingBox(), nullptr, nullptr));
				}
			}
		}

		// Axis comparator
		int axis = 0;

		auto axis_comparator = [&axis](const std::unique_ptr<const BVHnode>& a, const std::unique_ptr<const BVHnode>& b)
		{
			switch (axis)
			{
			case 0:
				return a->GetAxisAlignedBoundingBox().GetMinimum().x < b->GetAxisAlignedBoundingBox().GetMinimum().x;
			case 1:
				return a->GetAxisAlignedBoundingBox().GetMinimum().y < b->GetAxisAlignedBoundingBox().GetMinimum().y;
			default:
				return a->GetAxisAlignedBoundingBox().GetMinimum().z < b->GetAxisAlignedBoundingBox().GetMinimum().z;
			}
		};

		// First sort based on axis
		std::sort(nodes.begin(), nodes.end(), axis_comparator);

		m_treeLevels = 1;
		std::vector<std::unique_ptr<const BVHnode>> upperLevel;
		upperLevel.reserve(nodes.size());

		while (nodes.size() > 1)
		{
			for (uint i = 1; i < nodes.size(); i += 2)
			{
				const BVHnode* left = nodes[i - 1].release();
				const BVHnode* right = nodes[i].release();
				const AxisAlignedBoundingBox box = left->GetAxisAlignedBoundingBox() + right->GetAxisAlignedBoundingBox();

				upperLevel.emplace_back(std::make_unique<BVHnode>(nullptr, box, left, right));
			}

			if (nodes.size() % 2 == 1)
			{
				const BVHnode* left = nodes.back().release();
				upperLevel.emplace_back(std::make_unique<BVHnode>(nullptr, left->GetAxisAlignedBoundingBox(), left, nullptr));
			}

			nodes = std::move(upperLevel);
			upperLevel.clear();
			++m_treeLevels;

			axis = axis + 1 > 2 ? 0 : axis + 1;
			std::sort(nodes.begin(), nodes.end(), axis_comparator);
		}

		m_root = std::move(nodes.front());
	}

	void BoundingVolumeHierarchy::DebugPrint()
	{
		std::vector<const BVHnode*> nodes;

		nodes.push_back(m_root.get());

		std::cout << std::endl;

		while (nodes.size() > 0)
		{
			std::cout << std::endl;

			std::vector<const BVHnode*> followingLevel;

			for (const auto* n : nodes)
			{
				if (n == nullptr)
				{
					std::cout << "null    ";
				}
				else
				{
					const auto* p = n->GetPrimitive();
					if (p)
					{
						std::cout << static_cast<uint>(p->GetType()) << "    ";
					}
					else
					{
						std::cout << "o    ";

						followingLevel.push_back(n->GetLeft());
						followingLevel.push_back(n->GetRight());
					}
				}
			}

			nodes = followingLevel;
		}

		std::cout << std::endl;
	}

}
