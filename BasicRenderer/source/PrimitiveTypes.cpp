#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	void Sphere::ProcessForRendering(const Transform& transform)
	{
		m_pos = m_originalPos + transform.GetPosition();
		m_radius = m_originalRadius * transform.GetScale().x;
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}
	Primitive* Sphere::CloneForRendering() const
	{
		return new Sphere(*this);
	}
	AxisAlignedBoundingBox Sphere::UpdateAxisAlignedBoundingBox() const
	{
		return AxisAlignedBoundingBox(m_pos - Vector3(m_radius, m_radius, m_radius), m_pos + Vector3(m_radius, m_radius, m_radius));
	}

	void Plane::ProcessForRendering(const Transform& transform)
	{
		m_centre = transform.GetPosition();
		m_normal = transform.Up();
		m_boundingBox = UpdateAxisAlignedBoundingBox();
	}

	Primitive* Plane::CloneForRendering() const
	{
		return new Plane(*this);
	}

	AxisAlignedBoundingBox Plane::UpdateAxisAlignedBoundingBox() const
	{
		AxisAlignedBoundingBox box = m_boundingBox;
		box.m_size = -1.f; // special case that is always hit to make infinite planes work easily
		return box;
	}
}


