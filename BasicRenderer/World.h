#pragma once

#include <vector>

class World
{
public:
	//TODO implement it properly
	std::vector<class Hitable*> hierarchy;

	bool GetHit(const class Ray& r, float tMin, float tMax, struct HitResult& hit) const;
};