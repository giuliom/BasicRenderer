#pragma once

#include <memory>
#include <optional>
#include <string>
#include "Transform.h"
#include "DrawableInstance.h"

namespace BasicRenderer
{
	class Material;

	class SceneObject
	{
	protected:
		static uint m_idCounter;

		uint m_id;
		std::string m_name;
		mutable Transform m_worldTransform;
		Transform m_transform;
		std::optional<DrawableInstance> m_drawable;
		bool m_enabled;
		bool m_visible;

		SceneObject(const SceneObject& obj);

	public:
		SceneObject(const std::string& name = "");
		SceneObject(DrawableInstance drawable, const std::string& name = "");
		SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, const std::string& name = "");
		SceneObject(SceneObject&& obj) noexcept;
		virtual ~SceneObject();

		SceneObject& operator=(const SceneObject& obj) = delete;
		SceneObject& operator=(SceneObject&& obj) = delete;

		virtual void Update(const TimeDuration& deltaTime);
		SceneObject Clone() const;

		inline uint GetId() const noexcept										{ return m_id; }
		inline const std::string& GetName() const noexcept						{ return m_name; }
		inline Transform& GetTransform() noexcept								{ return m_transform; }
		inline const Transform& GetWorldTransform() const noexcept				{ return m_worldTransform; }
		inline const std::optional<DrawableInstance>& GetDrawableInstance() const noexcept { return m_drawable; }
		inline bool GetEnabled() const noexcept									{ return m_enabled; }
		inline bool GetVisible() const noexcept									{ return m_visible; }
		inline void SetEnabled(bool enabled) noexcept							{ m_enabled = enabled; }
		inline void SetVisible(bool visible) noexcept							{ m_visible = visible; }

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
