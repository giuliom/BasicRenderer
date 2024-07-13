#pragma once

#include <vector>
#include <memory>
#include "Transform.h"
#include "Primitive.h"
#include "Mesh.h"

namespace BasicRenderer
{
	class SceneObject
	{
	protected:
		static uint m_idCounter;

		uint m_id;
		std::string m_name;
		mutable Transform m_worldTransform;
		Transform m_transform;
		std::vector<Transform*> m_children; //TODO Move to Transform?
		std::unique_ptr<Primitive> m_primitive;
		bool m_enabled;
		bool m_visible;

	public:
		SceneObject();
		SceneObject(Primitive* primitive, const std::string& name = "");
		SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, const std::string& name = "");
		SceneObject(const SceneObject& obj);
		SceneObject(SceneObject&& obj) noexcept;
		virtual ~SceneObject();

		virtual void Update(const TimeDuration& deltaTime);

		inline uint GetId() const							{ return m_id; }
		inline const std::string& GetName() const			{ return m_name; }
		inline Transform& GetTransform()					{ return m_transform; }
		inline const Transform& GetWorldTransform() const	{ return m_worldTransform; }
		inline const Primitive* GetPrimitive() const		{ return m_primitive.get(); }
		inline Primitive* GetPrimitive()					{ return m_primitive.get(); }
		inline bool GetEnabled() const						{ return m_enabled; }
		inline bool GetVisible() const						{ return m_visible; }
		inline void SetEnabled(bool enabled)				{ m_enabled = enabled; }
		inline void SetVisible(bool visible)				{ m_visible = visible; }

	protected:

		virtual void UpdateTransform()
		{
			Transform* parent = m_transform.GetParent();

			if (m_transform.isDirty())
			{
				if (parent == nullptr)
				{
					m_worldTransform = m_transform;
				}
				else
				{
					m_worldTransform = m_transform.Combine(*parent);
				}

				for (auto t : m_children)
				{
					t->SetDirty(true);
				}

				m_worldTransform.SetDirty(true);
			}
		}
	};
}


