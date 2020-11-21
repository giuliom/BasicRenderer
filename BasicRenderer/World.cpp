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

	void World::ProcessForRendering()
	{
		std::vector<const Primitive*> primitives;

		for (auto& obj : m_objectList)
		{
			obj.second->ProcessForRendering();
			primitives.push_back(obj.second.get());
		}

		// TODO partial rebuilding of the bvh?
		m_bvh.Build(primitives);
	}

	const Primitive* World::OldRaycast(const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal) const
	{
		const Primitive* anyHit = nullptr;
		float closestHit = tMax;

		HitResult tempHit;
		tempHit.t = tMax;

		for (const auto& obj : m_objectList)
		{
			if (obj.second->GetHit(r, tMin, tMax, tempHit))
			{
				if (tempHit.t < closestHit)
				{
					closestHit = tempHit.t;
					hitNormal = tempHit.normal;
					anyHit = obj.second.get();
				}
			}
		}

		hitPosition = r.GetPoint(closestHit);

		return anyHit;
	}

	const Primitive* World::Raycast(const Ray& r, float tMin, float tMax, Vector3& hitPosition, Vector3& hitNormal) const
	{
		const Primitive* anyHit = nullptr;
		HitResult hit;
		anyHit = m_bvh.GetHit(r, tMin, tMax, hit);

		if (anyHit != nullptr)
		{
			hitPosition = r.GetPoint(hit.t);
			hitNormal = hit.normal;
		}
	
		return anyHit;
	}
}
