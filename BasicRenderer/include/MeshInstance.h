#pragma once

#include "Global.h"
#include "Mesh.h"
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	class MeshInstance
	{
		PrimitiveType m_type;
		std::optional<PrimitiveData> m_originalPrimitive;
		std::shared_ptr<Mesh> m_originalMesh;
		std::shared_ptr<Material> m_material;
		PrimitiveList m_primitives;
		AxisAlignedBoundingBox m_boundingBox;

		AxisAlignedBoundingBox UpdateAxisAlignedBoundingBox() const;

	public:

		template<typename T>
			requires (std::derived_from<T, Primitive> && !std::same_as<T, Primitive>)
		MeshInstance(const T& primitive, std::shared_ptr<Material> mat = nullptr)
		: m_type(primitive.GetType())
		, m_originalPrimitive(PrimitiveData(primitive))
		, m_originalMesh(nullptr)
		, m_material(mat)
		, m_primitives()
		, m_boundingBox()
		{
			auto clone = std::make_unique<T>(primitive);
			clone->SetMaterial(mat.get());
			m_primitives.push_back(std::move(clone));
			m_boundingBox = UpdateAxisAlignedBoundingBox();
		}

		MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat = nullptr);
		MeshInstance(const MeshInstance& other);
		MeshInstance& operator=(const MeshInstance&) = delete;

		const PrimitiveList& ProcessForRendering(Transform& transform);

		auto GetType() const noexcept { return m_type; }
		size_t NumPrimitives() const noexcept { return m_primitives.size(); }
		const Primitive& GetPrimitive(size_t index) const noexcept { return *m_primitives[index]; }
		const PrimitiveList& GetPrimitives() const noexcept { return m_primitives; }
		const std::shared_ptr<Material>& GetMaterial() const noexcept { return m_material; }

		bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const noexcept
		{
			outHit.t = tMax;
			bool hit = false;
			const auto numPrimitives = m_primitives.size();

			for (uint i = 0; i < numPrimitives; i++)
			{
				if (m_primitives[i]->GetHit(r, tMin, outHit.t, outHit))
				{
					hit = true;
				}
			}

			return hit;
		}
	};
}