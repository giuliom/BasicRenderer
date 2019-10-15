#pragma once

#include <vector>
#include "Vertex.h"
#include "DirectionalLight.h"

class Primitive;
class Ray;

class World
{
public:

	DirectionalLight sun;
	float ambientLightIntensity = 1.0f;
	Color ambientLightColor{ 1.f, 1.f, 1.f };

	//TODO implement it properly
	std::vector<Primitive*> hierarchy;

	void ProcessForRendering();
	const Primitive* GetHit(const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal) const;
};