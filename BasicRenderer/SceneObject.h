#pragma once

#include <vector>
#include "Transform.h"
#include "Mesh.h"

class SceneObject
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

};

