#pragma once

#include <vector>
#include <memory>
#include "Transform.h"
#include "Hitable.h"
#include "Mesh.h"

class SceneObject : public Hitable
{
protected:
	std::shared_ptr<Mesh> mesh;
	mutable Transform worldTransform;
	std::vector<Transform*> children; //TODO Move to Transform?

public:
	Transform transform;
	bool dirty = true;

public:
	SceneObject() : Hitable() {}
	SceneObject(Material* mat) : Hitable(mat) {}
	SceneObject(std::shared_ptr<Mesh> mesh_) : mesh(mesh_) {};
	SceneObject(std::shared_ptr<Mesh> mesh_, Material* mat) : Hitable(mat), mesh(mesh_) {} ;
	SceneObject(const SceneObject& obj) : Hitable(obj), mesh(obj.mesh), transform(obj.transform) {}
	virtual ~SceneObject();

	inline std::shared_ptr<Mesh> const GetMesh() const { return mesh; } 
	Transform& UpdateWorldTransform() const;

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;
};

