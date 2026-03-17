#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	void Sphere::ProcessForRendering(const Transform& transform)
	{
		m_pos = m_originalPos + transform.GetWorldPosition();
		Matrix4 wm = transform.GetWorldMatrix();
		float worldScaleX = Vector3(wm.x1, wm.y1, wm.z1).Length();
		m_radius = m_originalRadius * worldScaleX;
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
		m_centre = transform.GetWorldPosition();
		Matrix4 wm = transform.GetWorldMatrix();
		m_normal = Vector3(wm.x2, wm.y2, wm.z2).Normalize();
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


