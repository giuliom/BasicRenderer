#include "MeshInstance.h"
#include "PrimitiveTypes.h"
#include "Face.h"

namespace BasicRenderer
{
	MeshInstance::MeshInstance(const MeshInstance& other)
		: m_type(other.m_type)
		, m_originalPrimitive(other.m_originalPrimitive)
		, m_originalMesh(other.m_originalMesh)
		, m_material(other.m_material)
		, m_boundingBox(other.m_boundingBox)
	{
		if (m_originalMesh)
		{
			m_faces.reserve(m_originalMesh->NumFaces());
			for (const auto& f : m_originalMesh->GetFaces())
			{
				m_faces.push_back(f);
				m_faces.back().SetMaterial(m_material.get());
			}
		}
		else if (m_originalPrimitive.has_value())
		{
			std::visit([this](const auto& prim) {
				using T = std::decay_t<decltype(prim)>;
				if constexpr (std::same_as<T, Face>) {
					m_faces.push_back(prim);
					m_faces.back().SetMaterial(m_material.get());
				} else if constexpr (std::same_as<T, Sphere>) {
					m_sphere = prim;
					m_sphere->SetMaterial(m_material.get());
				} else if constexpr (std::same_as<T, Plane>) {
					m_plane = prim;
					m_plane->SetMaterial(m_material.get());
				}
			}, m_originalPrimitive.value());
		}
	}

	MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat)
		: m_type(PrimitiveType::FACE)
		, m_originalPrimitive(std::nullopt)
		, m_originalMesh(mesh)
		, m_material(mat)
		, m_boundingBox()
	{
		assert(mesh != nullptr);
		m_faces.reserve(mesh->NumFaces());
		for (const auto& f : mesh->GetFaces())
		{
			m_faces.push_back(f);
			m_faces.back().SetMaterial(mat.get());
		}
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}

	void MeshInstance::ProcessForRendering(Transform& transform)
	{
		if (transform.isDirty())
		{
			const Matrix4& worldMatrix = transform.GetWorldMatrix();

			switch (m_type)
			{
				case PrimitiveType::FACE:
				{
					for (size_t i = 0; i < m_faces.size(); i++)
					{
						const auto& originalFace = m_originalMesh->GetFaces()[i];
						m_faces[i] = BasicRenderer::ProcessForRendering(originalFace, worldMatrix);
					}
					break;
				}
				case PrimitiveType::SPHERE:
				{
					const auto& originalSphere = std::get<Sphere>(m_originalPrimitive.value());
					m_sphere = BasicRenderer::ProcessForRendering(originalSphere, transform);
					break;
				}
				case PrimitiveType::PLANE:
				{
					const auto& originalPlane = std::get<Plane>(m_originalPrimitive.value());
					m_plane = BasicRenderer::ProcessForRendering(originalPlane, worldMatrix);
					break;
				}
			}

			m_boundingBox = UpdateAxisAlignedBoundingBox();
			transform.SetDirty(false);
		}
	}

	AxisAlignedBoundingBox MeshInstance::UpdateAxisAlignedBoundingBox() const
	{
		switch (m_type)
		{
		case PrimitiveType::FACE:
		{
			if (m_faces.empty()) return m_boundingBox;

			Vector3 min = m_faces[0].GetAxisAlignedBoundingBox().GetMinimum();
			Vector3 max = m_faces[0].GetAxisAlignedBoundingBox().GetMaximum();

			for (const auto& f : m_faces)
			{
				const AxisAlignedBoundingBox& bbox = f.GetAxisAlignedBoundingBox();
				const Vector3& bboxMin = bbox.GetMinimum();
				const Vector3& bboxMax = bbox.GetMaximum();

				if (bboxMin.x < min.x) min.x = bboxMin.x;
				if (bboxMin.y < min.y) min.y = bboxMin.y;
				if (bboxMin.z < min.z) min.z = bboxMin.z;

				if (bboxMax.x > max.x) max.x = bboxMax.x;
				if (bboxMax.y > max.y) max.y = bboxMax.y;
				if (bboxMax.z > max.z) max.z = bboxMax.z;
			}

			return AxisAlignedBoundingBox(min, max);
		}
		case PrimitiveType::SPHERE:
			if (m_sphere) return m_sphere->GetAxisAlignedBoundingBox();
			break;
		case PrimitiveType::PLANE:
			if (m_plane) return m_plane->GetAxisAlignedBoundingBox();
			break;
		}

		return m_boundingBox;
	}
}