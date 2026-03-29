#include "World.h"
#include "Ray.h"
#include "PrimitiveTypes.h"
#include "Face.h"
#include "MeshInstance.h"
#include "SceneObject.h"

namespace BasicRenderer
{
	World::~World()
	{

	}

	void World::Add(std::unique_ptr<SceneObject> obj, Transform* parent)
	{
		if (parent != nullptr)
		{
			parent->AddChild(obj->GetTransform());
		}
		else
		{
			m_root.AddChild(obj->GetTransform());
		}
		m_objectList[obj->GetId()].reset(obj.release());
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
		std::vector<Transform*> stack;
		stack.push_back(&m_root);

		while (!stack.empty())
		{
			Transform* node = stack.back();
			stack.pop_back();

			SceneObject* obj = node->GetObject();
			if (obj != nullptr && obj->GetEnabled())
			{
				obj->Update(deltaTime);
			}

			for (auto* child : node->GetChildren())
			{
				stack.push_back(child);
			}
		}
	}

	InstanceList World::ProcessForRendering()
	{
		InstanceList outProcessed;
		outProcessed.reserve(m_objectList.size());

		std::vector<Transform*> stack;
		stack.push_back(&m_root);

		while (!stack.empty())
		{
			Transform* node = stack.back();
			stack.pop_back();

			SceneObject* obj = node->GetObject();
			if (obj != nullptr && obj->GetEnabled() && obj->GetVisible() && obj->GetMeshInstance() != nullptr)
			{
				auto instance = obj->GetMeshInstance();
				instance->ProcessForRendering(obj->GetTransform());
				outProcessed.push_back(instance);
				obj->GetTransform().SetDirty(false);
			}

			for (auto* child : node->GetChildren())
			{
				stack.push_back(child);
			}
		}

		return outProcessed;
	}

	[[deprecated("Raycast by brute forcing all primitives")]]
	const Primitive* World::OldRaycast(const Ray& r, float tMin, float tMax, HitResult& outHit) const
	{
		const Primitive* anyHit = nullptr;
		float closestHit = tMax;

		outHit.t = tMax;

		for (const auto& [id, obj] : m_objectList)
		{
			const auto instance = obj->GetMeshInstance();
			if (instance && instance->GetHit(r, tMin, tMax, outHit))
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
