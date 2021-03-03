#include "MeshInstance.h"


namespace BasicRenderer
{
	MeshInstance::MeshInstance(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material> mat, const std::string& name)
		: Primitive(mesh->m_faces, mat, name)
		, m_originalMesh(mesh)
	{
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}

	void MeshInstance::ProcessForRendering(const Transform& transform)
	{
		if (transform.isDirty())
		{
			for (int i = 0; i < m_faces.size(); ++i)
			{
				Face f = m_originalMesh->GetFace(i);
				ToMatrixSpace(f, transform.m_matrix);
				m_faces[i] = f;
			}

			m_boundingBox = UpdateAxisAlignedBoundingBox();
		}
	}

	AxisAlignedBoundingBox MeshInstance::UpdateAxisAlignedBoundingBox() const
	{
		if (m_faces.size() > 0)
		{
			Vector3 min = m_faces[0].v0.pos;
			Vector3 max = m_faces[0].v1.pos;

			for (const auto& f : m_faces)
			{
				for (uint i = 0; i < 3; ++i)
				{
					const Vector3& vertexPos = ((&f.v0) + i)->pos;

					if (vertexPos.x < min.x)
					{
						min.x = vertexPos.x;
					}
					if (vertexPos.y < min.y)
					{
						min.y = vertexPos.y;
					}
					if (vertexPos.z < min.z)
					{
						min.z = vertexPos.z;
					}

					if (vertexPos.x > max.x)
					{
						max.x = vertexPos.x;
					}
					if (vertexPos.y > max.y)
					{
						max.y = vertexPos.y;
					}
					if (vertexPos.z > max.z)
					{
						max.z = vertexPos.z;
					}
				}
			}

			return AxisAlignedBoundingBox(min, max);
		}

		return m_boundingBox;
	}
}