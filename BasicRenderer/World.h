#pragma once

#include <vector>
#include <memory>
#include "Vertex.h"
#include "DirectionalLight.h"

class Primitive;
class Ray;

typedef std::vector<std::shared_ptr<Primitive>> Hierarchy;

class World
{
protected:

	//TODO implement it properly
	Hierarchy hierarchy;

public:

	World() {}
	~World();

	DirectionalLight sun;
	float ambientLightIntensity = 1.0f;
	Color ambientLightColor{ 1.f, 1.f, 1.f };

	inline const Hierarchy& GetHierarchy() const { return hierarchy; }

	void Add(Primitive* obj);
	void ProcessForRendering();
	const Primitive* GetHit(const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal) const;
};