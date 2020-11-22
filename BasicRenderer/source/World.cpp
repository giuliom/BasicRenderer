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
		for (auto& obj : m_objectList)
		{
			obj.second->ProcessForRendering();
		}

		// TODO partial rebuilding of the bvh?
		m_bvh.Build(m_objectList.begin(), m_objectList.end());
	}

	const Primitive* World::OldRaycast(const Ray& r, float tMin, float tMax, HitResult& outHit) const
	{
		const Primitive* anyHit = nullptr;
		float closestHit = tMax;

		outHit.t = tMax;

		for (const auto& obj : m_objectList)
		{
			if (obj.second->GetHit(r, tMin, tMax, outHit))
			{
				if (outHit.t < closestHit)
				{
					closestHit = outHit.t;
					anyHit = outHit.primitive;
				}
			}
		}

		outHit.t = closestHit;

		return anyHit;
	}
	
}
