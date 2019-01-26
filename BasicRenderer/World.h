#pragma once

#include <vector>
#include "DirectionalLight.h"

class World
{
public:

	DirectionalLight sun;

	//TODO implement it properly
	std::vector<class Hitable*> hierarchy;

	bool GetHit(const class Ray& r, float tMin, float tMax, struct HitResult& hit) const;
};