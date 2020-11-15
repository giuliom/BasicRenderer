#include "SceneObject.h"
#include "Face.h"


namespace BasicRenderer
{
	SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, Material* mat)
		: Primitive(mat)
		, m_mesh(mesh)
		, m_transformedFaces(m_mesh ? m_mesh->NumFaces() : 0)
	{
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}

	SceneObject::SceneObject(const SceneObject& obj)
		: m_worldTransform(obj.m_worldTransform)
		, m_transform(obj.m_transform)
		, m_children(obj.m_children)
		, m_mesh(obj.m_mesh)
		, m_transformedFaces(obj.m_transformedFaces)
	{
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}

	SceneObject::SceneObject(SceneObject&& obj) noexcept
		: m_worldTransform(obj.m_worldTransform)
		, m_transform(obj.m_transform)
		, m_children(obj.m_children)
		, m_mesh(obj.m_mesh)
		, m_transformedFaces(obj.m_transformedFaces)
	{
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}

	SceneObject::~SceneObject()
	{
	}

	void SceneObject::ProcessForRendering()
	{
		Transform* parent = m_transform.GetParent();
		bool updateFaces = false;

		if (m_transform.isDirty())
		{
			if (parent == nullptr)
			{
				m_worldTransform = m_transform;
			}
			else
			{
				m_worldTransform = m_transform.Combine(*parent);
			}

			for (auto t : m_children)
			{
				t->SetDirty(true);
			}
		}

		if (m_transform.isDirty())
		{
			for (int i = 0; i < NumFaces(); ++i)
			{
				Face f = m_mesh->GetFace(i);
				ToMatrixSpace(f, m_worldTransform.m_matrix);
				m_transformedFaces[i] = f;
			}

			m_boundingBox = UpdateAxisAlignedBoundingBox();

			m_transform.SetDirty(false);
		}
	}

	AxisAlignedBoundingBox SceneObject::UpdateAxisAlignedBoundingBox() const
	{
		if (m_transformedFaces.size() > 0)
		{
			Vector3 min = m_transformedFaces[0].v0.pos;
			Vector3 max = m_transformedFaces[0].v1.pos;

			for (const auto& f : m_transformedFaces)
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
						min.x = vertexPos.x;
					}
					if (vertexPos.y > max.y)
					{
						min.y = vertexPos.y;
					}
					if (vertexPos.z > max.z)
					{
						min.z = vertexPos.z;
					}
				}
			}

			return AxisAlignedBoundingBox(min, max);
		}

		return m_boundingBox;
	}
}