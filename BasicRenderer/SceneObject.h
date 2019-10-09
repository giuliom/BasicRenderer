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
	Face* transformedFaces = nullptr;
	mutable Transform worldTransform;
	std::vector<Transform*> children; //TODO Move to Transform?

public:
	Transform transform;
	bool dirty = true;

public:
	SceneObject() = delete;
	SceneObject(Material* mat) = delete;
	SceneObject(std::shared_ptr<Mesh> mesh_);
	SceneObject(std::shared_ptr<Mesh> mesh_, Material* mat);
	SceneObject(const SceneObject& obj);
	SceneObject(SceneObject&& obj);
	virtual ~SceneObject();

	//TODO define assigments

	inline std::shared_ptr<Mesh> const GetMesh() const { return mesh; }
	inline const Transform& GetWorldTransform() const { return worldTransform; }

	void ProcessForRendering() const;
	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;
};

