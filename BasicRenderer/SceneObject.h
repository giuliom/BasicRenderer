#pragma once

#include <vector>
#include "Transform.h"
#include "Hitable.h"
#include "Mesh.h"

class SceneObject : public Hitable
{
protected:
	Mesh* mesh = nullptr;
	mutable Transform worldTransform;
	std::vector<Transform*> children; //TODO Move to Transform?

public:
	Transform transform;
	bool dirty = true;

public:
	SceneObject(Mesh* mesh_) : mesh(mesh_) {} ;
	SceneObject(const SceneObject& obj) : mesh(obj.mesh), transform(obj.transform) {}
	~SceneObject();

	inline const Mesh* const GetMesh() const { return mesh; } 
	Transform& UpdateWorldTransform() const;

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override { return false; }

};

