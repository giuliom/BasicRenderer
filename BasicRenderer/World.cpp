#include "World.h"
#include "Ray.h"
#include "Primitive.h"

World::~World()
{
	for (auto* obj : hierarchy)
	{
		delete obj;
	}
}

void World::Add(Primitive* obj)
{
	if (obj != nullptr)
	{
		hierarchy.emplace_back(obj);
	}
}

void World::ProcessForRendering()
{
	for (auto& obj : hierarchy)
	{
		obj->ProcessForRendering();
	}
}

const Primitive* World::GetHit(const Ray & r, float tMin, float tMax, Vector3 & hitPosition, Vector3& hitNormal) const
{
	const Primitive* anyHit = nullptr;
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
