#include "World.h"
#include "Ray.h"
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
		auto it = m_objectList.find(id);

		if (it != m_objectList.end())
		{
			return it->second.get();
		}
		return nullptr;
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

	DrawableInstanceList World::ProcessForRendering(FaceBuffer& outFaceBuffer)
	{
		std::vector<SceneObject*> visibleObjects;
		visibleObjects.reserve(m_objectList.size());

		std::vector<Transform*> stack;
		stack.push_back(&m_root);

		while (!stack.empty())
		{
			Transform* node = stack.back();
			stack.pop_back();

			SceneObject* obj = node->GetObject();
			if (obj != nullptr && obj->GetEnabled() && obj->GetVisible() && obj->GetDrawableInstance().has_value())
			{
				visibleObjects.push_back(obj);
			}

			for (auto* child : node->GetChildren())
			{
				stack.push_back(child);
			}
		}

		size_t totalFaces = 0u;
		for (const SceneObject* obj : visibleObjects)
		{
			totalFaces += obj->GetDrawableInstance()->NumSourceFaces();
		}

		DrawableInstanceList outProcessed;
		outProcessed.reserve(visibleObjects.size());
		outFaceBuffer.clear();
		outFaceBuffer.reserve(totalFaces);

		for (SceneObject* obj : visibleObjects)
		{
			outProcessed.emplace_back(obj->GetDrawableInstance()->ProcessForRendering(obj->GetTransform(), outFaceBuffer));
		}

		return outProcessed;
	}
}
