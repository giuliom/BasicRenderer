#pragma once

#include <vector>
#include <memory>
#include "Transform.h"
#include "Mesh.h"
#include "MeshInstance.h"

namespace BasicRenderer
{
	class Primitive;
	class Material;
	class MeshInstance;

	class SceneObject
	{
	protected:
		static uint m_idCounter;

		uint m_id;
		std::string m_name;
		mutable Transform m_worldTransform;
		Transform m_transform;
		std::shared_ptr<MeshInstance> m_meshInstance;
		bool m_enabled;
		bool m_visible;

		SceneObject(const SceneObject& obj);

	public:
		SceneObject(const std::string& name = "");
		SceneObject(const MeshInstance& instance, std::shared_ptr<Material> mat, const std::string& name = "");
		SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, const std::string& name = "");
		SceneObject(SceneObject&& obj) noexcept;
		virtual ~SceneObject();

		SceneObject& operator=(const SceneObject& obj) = delete;
		SceneObject& operator=(SceneObject&& obj) = delete;

		virtual void Update(const TimeDuration& deltaTime);
		SceneObject Clone() const;

		inline uint GetId() const											{ return m_id; }
		inline const std::string& GetName() const							{ return m_name; }
		inline Transform& GetTransform()									{ return m_transform; }
		inline const Transform& GetWorldTransform() const					{ return m_worldTransform; }
		inline const std::shared_ptr<MeshInstance>& GetMeshInstance() const { return m_meshInstance; }
		inline bool GetEnabled() const										{ return m_enabled; }
		inline bool GetVisible() const										{ return m_visible; }
		inline void SetEnabled(bool enabled)								{ m_enabled = enabled; }
		inline void SetVisible(bool visible)								{ m_visible = visible; }

	protected:

		virtual void UpdateTransform()
		{
			if (m_transform.isDirty())
			{
				for (auto t : m_transform.m_children)
				{
					t->SetDirty(true);
				}
			}
		}
	};
}


