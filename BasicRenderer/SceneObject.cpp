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
