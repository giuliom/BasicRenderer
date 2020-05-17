#include "World.h"
#include "Ray.h"
#include "Primitive.h"
#include "PrimitiveTypes.h"
#include "Face.h"
#include "SceneObject.h"

World::~World()
{

}

void World::Add(Primitive* obj)
{
	if (obj != nullptr)
	{
		hierarchy.emplace_back(obj);
	}
}

void World::ProcessForRendering(const Matrix4& projection, const Matrix4& view)
{
	for (auto& obj : hierarchy)
	{
		obj->ProcessForRendering(projection, view);
	}
}

const Primitive* World::Raycast(const Ray & r, float tMin, float tMax, Vector3 & hitPosition, Vector3& hitNormal) const
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
				anyHit = obj.get();
			}
		}
	}

	hitPosition = r.GetPoint(closestHit);

	return anyHit;
}
