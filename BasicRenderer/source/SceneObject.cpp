#include "SceneObject.h"
#include "Face.h"
#include "MeshInstance.h"


namespace BasicRenderer
{
	uint SceneObject::m_idCounter = 0;

	SceneObject::SceneObject()
		: m_id(m_idCounter++)
		, m_name("SceneObject_" + std::to_string(m_id))
		, m_worldTransform()
		, m_transform()
		, m_children()
		, m_primitive(nullptr)
		, m_enabled(true)
		, m_visible(true)
	{
	}

	SceneObject::SceneObject(Primitive* primitive, const std::string& name)
		: m_id(m_idCounter++)
		, m_name(name.size() > 0 ? name : "SceneObject_" + std::to_string(m_id))
		, m_worldTransform()
		, m_transform()
		, m_children()
		, m_primitive(std::move(primitive))
		, m_enabled(true)
		, m_visible(true)
	{
	}

	SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, const std::string& name)
		: m_id(m_idCounter++)
		, m_name(name.size() > 0 ? name : "SceneObject_" + std::to_string(m_id))
		, m_worldTransform()
		, m_transform()
		, m_children()
		, m_primitive(new MeshInstance(mesh, mat, name))
		, m_enabled(true)
		, m_visible(true)
	{
	}

	SceneObject::SceneObject(SceneObject&& obj) noexcept
		: m_id(obj.m_id)
		, m_name(obj.m_name)
		, m_worldTransform(obj.m_worldTransform)
		, m_transform(obj.m_transform)
		, m_children(obj.m_children)
		, m_primitive(std::move(obj.m_primitive))
		, m_enabled(obj.m_enabled)
		, m_visible(obj.m_visible)
	{
	}

	SceneObject::~SceneObject()
	{
	}

	void SceneObject::Update(const TimeDuration& deltaTime)
	{
		UpdateTransform();
	}
}