#include "SceneObject.h"
#include "Face.h"


namespace BasicRenderer
{
	SceneObject::SceneObject(std::shared_ptr<Mesh> mesh)
		: m_mesh(mesh)
		, m_transformedFaces(m_mesh ? m_mesh->NumFaces() : 0)
	{

	}

	SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, Material* mat)
		: Primitive(mat)
		, m_mesh(mesh)
		, m_transformedFaces(m_mesh ? m_mesh->NumFaces() : 0)
	{

	}

	SceneObject::SceneObject(const SceneObject& obj)
		: m_worldTransform(obj.m_worldTransform)
		, m_transform(obj.m_transform)
		, m_children(obj.m_children)
		, m_mesh(obj.m_mesh)
		, m_transformedFaces(obj.m_transformedFaces)
	{
	}

	SceneObject::SceneObject(SceneObject&& obj)
		: m_worldTransform(obj.m_worldTransform)
		, m_transform(obj.m_transform)
		, m_children(obj.m_children)
		, m_mesh(obj.m_mesh)
		, m_transformedFaces(obj.m_transformedFaces)
	{
	}

	SceneObject::~SceneObject()
	{
	}

	void SceneObject::ProcessForRendering()
	{
		Transform* parent = m_transform.GetParent();
		bool updateFaces = false;

		if (parent == nullptr)
		{
			m_worldTransform = m_transform;
		}
		else
		{
			if (m_transform.dirty)
			{
				m_worldTransform = m_transform.Combine(*parent);
				m_transform.dirty = false;
				for (auto t : m_children)
				{
					t->dirty = true;
				}
			}
		}

		if (m_transform.dirty)
		{
			for (int i = 0; i < NumFaces(); ++i)
			{
				Face f = m_mesh->GetFace(i);
				ToMatrixSpace(f, m_worldTransform.m);
				m_transformedFaces[i] = f;
			}
		}
	}

	bool SceneObject::GetHit(const Ray& r, float tMin, float tMax, float& tHit, Vector3& normalHit) const
	{
		tHit = tMax;
		float test;
		bool hit = false;

		if (GetMesh() != nullptr)
		{
			for (int i = 0; i < NumFaces(); i++)
			{
				const Face& f = GetTransformedFace(i);
				if (f.GetHit(r, tMin, tMax, test))
				{
					hit = true;
					if (test < tHit)
					{
						tHit = test;
						normalHit = f.normal;
					}
				}
			}
		}

		return hit;
	}
}