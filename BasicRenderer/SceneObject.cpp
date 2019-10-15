#include "SceneObject.h"




SceneObject::SceneObject(std::shared_ptr<Mesh> mesh_)
	: mesh(mesh_)
{
	transformedFaces = new Face[mesh->GetFacesCount()];
}

SceneObject::SceneObject(std::shared_ptr<Mesh> mesh_, Material* mat) 
	: Primitive(mat)
	, mesh(mesh_)
{
	transformedFaces = new Face[mesh->GetFacesCount()];
}

SceneObject::SceneObject(const SceneObject& obj)
	: mesh(obj.mesh)
	, worldTransform(obj.worldTransform)
	, children(obj.children)
	, transform(transform)
{
	delete[] transformedFaces;
	transformedFaces = new Face[mesh->GetFacesCount()];
	memcpy(transformedFaces, obj.transformedFaces, sizeof(Face) * mesh->GetFacesCount());
}

SceneObject::SceneObject(SceneObject&& obj)
	: mesh(obj.mesh)
	, worldTransform(obj.worldTransform)
	, children(obj.children)
	, transform(transform)
{
	delete[] transformedFaces;
	transformedFaces = obj.transformedFaces;
	obj.transformedFaces = nullptr;
}

SceneObject::~SceneObject()
{
	delete[] transformedFaces;
}

void SceneObject::ProcessForRendering() const
{
	Transform* parent = transform.GetParent();
	bool updateFaces = false;

	if (parent == nullptr)
	{ 
		worldTransform = transform; 
	}
	else
	{
		if (transform.dirty)
		{
			worldTransform = transform.Combine(*parent);
			transform.dirty = false;
			for (auto t : children)
			{
				t->dirty = true;
			}
		}
	}

	if (transform.dirty)
	{
		for (int i = 0; i < mesh->GetFacesCount(); ++i)
		{
			transformedFaces[i] = mesh->GetFaces()[i].ToMatrixSpace(worldTransform.m);
		}
	}
}

bool SceneObject::GetHit(const Ray & r, float tMin, float tMax, float& tHit, Vector3& normalHit) const
{
	tHit = tMax;
	float test;
	bool hit = false;

	if (mesh != nullptr)
	{
		for (int i = 0; i < mesh->GetFacesCount(); i++)
		{
			if (transformedFaces[i].GetHit(r, tMin, tMax, test))
			{
				hit = true;
				if (test < tHit)
				{
					tHit = test;
					normalHit = transformedFaces[i].normal;
				}
			}
		}
	}

	return hit;
}
