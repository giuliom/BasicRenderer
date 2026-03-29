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
		, m_primitives()
		, m_boundingBox(other.m_boundingBox)
	{
		if (m_originalMesh)
		{
			m_primitives.reserve(m_originalMesh->NumFaces());
			for (const auto& f : m_originalMesh->GetFaces())
			{
				auto face = std::make_unique<Face>(f);
				face->SetMaterial(m_material.get());
				m_primitives.push_back(std::move(face));
			}
		}
		else if (m_originalPrimitive.has_value())
		{
			std::visit([this](const auto& prim) {
				using T = std::decay_t<decltype(prim)>;
				auto copy = std::make_unique<T>(prim);
				copy->SetMaterial(m_material.get());
				m_primitives.push_back(std::move(copy));
			}, m_originalPrimitive.value());
		}
	}

	MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat)
		: m_type(PrimitiveType::FACE)
		, m_originalPrimitive(std::nullopt)
		, m_originalMesh(mesh)
		, m_material(mat)
		, m_primitives()
		, m_boundingBox()
	{
		assert(mesh != nullptr);
		m_primitives.reserve(mesh->NumFaces());
		for (const auto& f : mesh->GetFaces())
		{
			auto face = std::make_unique<Face>(f);
			face->SetMaterial(mat.get());
			m_primitives.push_back(std::move(face));
		}
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}

	const PrimitiveList& MeshInstance::ProcessForRendering(const Transform& transform)
	{
		if (transform.isDirty())
		{
			for (size_t i = 0; i < m_primitives.size(); i++)
			{
				auto& p = m_primitives[i];
				switch (m_type)
				{
					case PrimitiveType::FACE:
					{
						auto& face = static_cast<Face&>(*p);
						const auto& originalFace = m_originalMesh->GetFaces()[i];	
						face = BasicRenderer::ProcessForRendering(originalFace, transform);
						break;
					}
					case PrimitiveType::SPHERE:
					{
						auto& sphere = static_cast<Sphere&>(*p);
						const auto& originalSphere = std::get<Sphere>(m_originalPrimitive.value());
						sphere = BasicRenderer::ProcessForRendering(originalSphere, transform);
						break;
					}
					case PrimitiveType::PLANE:
					{
						auto& plane = static_cast<Plane&>(*p);
						const auto& originalPlane = std::get<Plane>(m_originalPrimitive.value());
						plane = BasicRenderer::ProcessForRendering(originalPlane, transform);
						break;
					}
				}
			}

			m_boundingBox = UpdateAxisAlignedBoundingBox();
		}

		return m_primitives;
	}

	AxisAlignedBoundingBox MeshInstance::UpdateAxisAlignedBoundingBox() const
	{
		if (m_primitives.size() > 0)
		{
			Vector3 min = m_primitives[0]->GetAxisAlignedBoundingBox().GetMinimum();
			Vector3 max = m_primitives[0]->GetAxisAlignedBoundingBox().GetMaximum();

			for (const auto& prim : m_primitives)
			{
				const AxisAlignedBoundingBox& bbox = prim->GetAxisAlignedBoundingBox();
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

		return m_boundingBox;
	}
}