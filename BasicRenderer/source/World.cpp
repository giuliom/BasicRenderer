#include "World.h"
#include "Ray.h"
#include "PrimitiveTypes.h"
#include "Face.h"
#include "SceneObject.h"

namespace BasicRenderer
{
	World::~World()
	{

	}

	void World::Add(SceneObject* obj)
	{
		if (obj != nullptr)
		{
			m_objectList[obj->GetId()].reset(obj);
		}
	}

	bool World::Remove(const uint id)
	{
		return m_objectList.erase(id) > 0;
	}

	SceneObject* World::Find(const uint id)
	{
		return m_objectList[id].get();
	}

	const SceneObject* World::Find(const uint id) const
	{
		const auto& it = m_objectList.find(id);

		if (it != m_objectList.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	void World::Update(const TimeDuration& deltaTime)
	{
		for (auto& [id, obj] : m_objectList)
		{
			if (obj->GetEnabled())
			{
				obj->Update(deltaTime);
			}
		}
	}

	void World::ProcessForRendering(std::vector<Primitive*>& outProcessedPrimitives)
	{
		outProcessedPrimitives.reserve(m_objectList.size());

		for (auto& [id, obj] : m_objectList)
		{
			SceneObject& so = *obj;

			if (so.GetEnabled() && so.GetVisible())
			{
				Primitive* prim = so.GetPrimitive();

				if (prim != nullptr)
				{
					prim->ProcessForRendering(so.GetTransform());

					Primitive* clone = prim->CloneForRendering();
					outProcessedPrimitives.push_back(clone);
				}
			}

			so.GetTransform().SetDirty(false);
		}
	}

	const Primitive* World::OldRaycast(const Ray& r, float tMin, float tMax, HitResult& outHit) const
	{
		const Primitive* anyHit = nullptr;
		float closestHit = tMax;

		outHit.t = tMax;

		for (const auto& [id, obj] : m_objectList)
		{
			const Primitive* prim = obj->GetPrimitive();
			if (prim && prim->GetHit(r, tMin, tMax, outHit))
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
