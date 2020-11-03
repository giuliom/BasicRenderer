#include "World.h"
#include "Ray.h"
#include "Primitive.h"
#include "PrimitiveTypes.h"
#include "Face.h"
#include "SceneObject.h"

namespace BasicRenderer
{
	World::~World()
	{

	}

	void World::Add(Primitive* obj)
	{
		if (obj != nullptr && Find(obj->GetId()) == false)
		{
			m_objectList[obj->GetId()].reset(obj);
		}
	}

	bool World::Remove(const uint id)
	{
		return m_objectList.erase(id) > 0;
	}

	Primitive* World::Find(const uint id)
	{
		return m_objectList[id].get();
	}

	const Primitive* World::Find(const uint id) const
	{
		const auto& it = m_objectList.find(id);

		if (it != m_objectList.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	void World::BuildBoundingVolumeHierarchy()
	{
		// TODO implement BVH building
		//Fully rebuild it or partially? 
	}

	void World::ProcessForRendering()
	{
		for (auto& obj : m_objectList)
		{
			obj.second->ProcessForRendering();
		}

		BuildBoundingVolumeHierarchy();
	}

	const Primitive* World::Raycast(const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal) const
	{
		const Primitive* anyHit = nullptr;
		float closestHit = tMax;
		float tempHit = tMax;
		Vector3 tempNormal;

		for (const auto& obj : m_objectList)
		{
			if (obj.second->GetHit(r, tMin, tMax, tempHit, tempNormal))
			{
				if (tempHit < closestHit)
				{
					closestHit = tempHit;
					hitNormal = tempNormal;
					anyHit = obj.second.get();
				}
			}
		}

		hitPosition = r.GetPoint(closestHit);

		return anyHit;
	}
}
