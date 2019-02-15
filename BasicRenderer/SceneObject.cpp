#include "SceneObject.h"




SceneObject::~SceneObject()
{
	
}

Transform& SceneObject::UpdateWorldTransform() const
{
	Transform* parent = transform.GetParent();

	if (parent == nullptr)
	{ 
		worldTransform = transform; 
	}
	else if (transform.dirty)
	{
		worldTransform = transform.Combine(*parent);
		transform.dirty = false;
		for (auto t : children)
		{
			t->dirty = true;
		}
	}

	return worldTransform;
}

bool SceneObject::GetHit(const Ray & r, float tMin, float tMax, HitResult & result) const
{
	Matrix4 m = transform.GetMatrix();
	result.t = tMax;
	HitResult test;
	bool hit = false;

	if (mesh != nullptr)
	{
		const auto faces = mesh->GetFaces();
		int nfaces = mesh->GetFacesCount();

		for (int i = 0; i < nfaces; i++)
		{
			Face f = faces[i].ToMatrixSpace(m);
			if (f.GetHit(r, tMin, tMax, test))
			{
				hit = true;
				if (test.t < result.t)
				{
					result = test;
				}
			}
		}
	}

	result.material = this->GetMaterial();

	return hit;
}
