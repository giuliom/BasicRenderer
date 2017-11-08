#pragma once

#include "Transform.h"
#include "Mesh.h"

class SceneObject
{
public:
	SceneObject(Mesh* mesh_) : mesh(mesh_) {} ;
	SceneObject(const SceneObject& obj) : mesh(obj.mesh), transform(obj.transform) {}
	~SceneObject();

	Transform transform;
	Mesh* mesh = nullptr;
};

