#pragma once

#include "Transform.h"
#include "Mesh.h"

class SceneObject
{
public:
	SceneObject(Mesh* mesh_): mesh(mesh_) {}
	~SceneObject();

	Transform transform;
	Mesh* mesh = nullptr;
};

