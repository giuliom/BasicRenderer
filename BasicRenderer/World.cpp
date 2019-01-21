#include "World.h"
#include "Ray.h"
#include "Hitable.h"

bool World::GetHit(const Ray & r, float tMin, float tMax, HitResult & hit) const
{
	HitResult tempHit;
	bool anyHit = false;
	float closestHit = tMax;

	for (auto& obj : hierarchy)
	{
		if (obj->GetHit(r, tMin, tMax, tempHit))
		{
			anyHit = true;
			if (tempHit.t < closestHit)
			{
				closestHit = tempHit.t;
				hit = tempHit;
			}
		}
	}
	return anyHit;
}
