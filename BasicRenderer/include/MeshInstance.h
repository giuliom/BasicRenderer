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

		// Contiguous type-specific storage (replaces vector<unique_ptr<Primitive>>)
		std::vector<Face> m_faces;
		std::optional<Sphere> m_sphere;
		std::optional<Plane> m_plane;

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
		, m_boundingBox()
		{
			if constexpr (std::same_as<T, Face>) {
				m_faces.push_back(primitive);
				m_faces.back().SetMaterial(mat.get());
			} else if constexpr (std::same_as<T, Sphere>) {
				m_sphere = primitive;
				m_sphere->SetMaterial(mat.get());
			} else if constexpr (std::same_as<T, Plane>) {
				m_plane = primitive;
				m_plane->SetMaterial(mat.get());
			}
			m_boundingBox = UpdateAxisAlignedBoundingBox();
		}

		MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat = nullptr);
		MeshInstance(const MeshInstance& other);
		MeshInstance& operator=(const MeshInstance&) = delete;

		void ProcessForRendering(Transform& transform);

		auto GetType() const noexcept { return m_type; }

		size_t NumPrimitives() const noexcept
		{
			switch (m_type) {
			case PrimitiveType::FACE: return m_faces.size();
			case PrimitiveType::SPHERE: return m_sphere.has_value() ? 1 : 0;
			case PrimitiveType::PLANE: return m_plane.has_value() ? 1 : 0;
			}
			return 0;
		}

		const Primitive* GetPrimitive(size_t index) const noexcept
		{
			switch (m_type) {
			case PrimitiveType::FACE: return &m_faces[index];
			case PrimitiveType::SPHERE: return m_sphere.has_value() ? &m_sphere.value() : nullptr;
			case PrimitiveType::PLANE: return m_plane.has_value() ? &m_plane.value() : nullptr;
			}
			return nullptr;
		}
		
		const std::vector<Face>& GetFaces() const noexcept { return m_faces; }
		const std::shared_ptr<Material>& GetMaterial() const noexcept { return m_material; }

		bool GetHit(const Ray& r, float tMin, float tMax, HitResult& outHit) const noexcept
		{
			outHit.t = tMax;
			bool hit = false;

			switch (m_type) {
			case PrimitiveType::FACE:
				for (const auto& f : m_faces)
				{
					if (f.GetHit(r, tMin, outHit.t, outHit)) hit = true;
				}
				break;
			case PrimitiveType::SPHERE:
				if (m_sphere && m_sphere->GetHit(r, tMin, outHit.t, outHit)) hit = true;
				break;
			case PrimitiveType::PLANE:
				if (m_plane && m_plane->GetHit(r, tMin, outHit.t, outHit)) hit = true;
				break;
			}

			return hit;
		}
	};
}