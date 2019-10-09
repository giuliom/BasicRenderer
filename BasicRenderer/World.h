#pragma once

#include <vector>
#include "Vertex.h"
#include "DirectionalLight.h"

class World
{
public:

	DirectionalLight sun;
	float ambientLightIntensity = 1.0f;
	Color ambientLightColor{ 1.f, 1.f, 1.f };

	//TODO implement it properly
	std::vector<class Hitable*> hierarchy;

	void ProcessForRendering();
	bool GetHit(const class Ray& r, float tMin, float tMax, struct HitResult& hit) const;
};