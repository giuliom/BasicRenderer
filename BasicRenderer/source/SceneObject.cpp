#include "SceneObject.h"
#include "Face.h"
#include "MeshInstance.h"


namespace BasicRenderer
{
	uint SceneObject::m_idCounter = 0;

	SceneObject::SceneObject(const std::string& name)
		: m_id(m_idCounter++)
		, m_name(name.size() > 0 ? name : "SceneObject_" + std::to_string(m_id))
		, m_worldTransform()
		, m_transform()
		, m_meshInstance(nullptr)
		, m_enabled(true)
		, m_visible(true)
	{
		m_transform.m_object = this;
	}

	SceneObject::SceneObject(const MeshInstance& instance, std::shared_ptr<Material> mat, const std::string& name)
		: m_id(m_idCounter++)
		, m_name(name.size() > 0 ? name : "SceneObject_" + std::to_string(m_id))
		, m_worldTransform()
		, m_transform()
		, m_meshInstance(std::make_shared<MeshInstance>(instance))
		, m_enabled(true)
		, m_visible(true)
	{
		m_transform.m_object = this;
	}

	SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, const std::string& name)
		: m_id(m_idCounter++)
		, m_name(name.size() > 0 ? name : "SceneObject_" + std::to_string(m_id))
		, m_worldTransform()
		, m_transform()
		, m_meshInstance(std::make_shared<MeshInstance>(mesh, mat))
		, m_enabled(true)
		, m_visible(true)
	{
		m_transform.m_object = this;
	}

	SceneObject::SceneObject(const SceneObject& obj)
		: m_id(m_idCounter++)
		, m_name(obj.m_name)
		, m_worldTransform(obj.m_worldTransform)
		, m_transform(obj.m_transform)
		, m_meshInstance(std::make_shared<MeshInstance>(*obj.m_meshInstance))
		, m_enabled(obj.m_enabled)
		, m_visible(obj.m_visible)
	{
		m_transform.m_object = this;
	}

	SceneObject::SceneObject(SceneObject&& obj) noexcept
		: m_id(obj.m_id)
		, m_name(std::move(obj.m_name))
		, m_worldTransform(std::move(obj.m_worldTransform))
		, m_transform(std::move(obj.m_transform))
		, m_meshInstance(std::move(obj.m_meshInstance))
		, m_enabled(obj.m_enabled)
		, m_visible(obj.m_visible)
	{
		m_transform.m_object = this;
	}

	SceneObject::~SceneObject()
	{
	}

	SceneObject SceneObject::Clone() const
	{
		SceneObject clone(*this);
		return clone;
	}

	void SceneObject::Update(const TimeDuration& deltaTime)
	{
		(void)deltaTime;
		UpdateTransform();
	}
}