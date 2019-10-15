#include "World.h"
#include "Ray.h"
#include "Primitive.h"

void World::ProcessForRendering()
{
	for (auto obj : hierarchy)
	{
		obj->ProcessForRendering();
	}
}

const Primitive* World::GetHit(const Ray & r, float tMin, float tMax, Vector3 & hitPosition, Vector3& hitNormal) const
{
	Primitive* anyHit = nullptr;
	float closestHit = tMax;
	float tempHit = tMax;
	Vector3 tempNormal;

	for (auto& obj : hierarchy)
	{
		if (obj->GetHit(r, tMin, tMax, tempHit, tempNormal))
		{
			
			if (tempHit < closestHit)
			{
				closestHit = tempHit;
				hitNormal = tempNormal;
				anyHit = obj;
			}
		}
	}

	hitPosition = r.GetPoint(closestHit);

	return anyHit;
}
