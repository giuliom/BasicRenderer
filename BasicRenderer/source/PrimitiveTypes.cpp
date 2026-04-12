#include "PrimitiveTypes.h"

namespace BasicRenderer
{	
	AxisAlignedBoundingBox Sphere::UpdateAxisAlignedBoundingBox() const noexcept
	{
		return AxisAlignedBoundingBox(m_pos - Vector3(m_radius, m_radius, m_radius), m_pos + Vector3(m_radius, m_radius, m_radius));
	}

	AxisAlignedBoundingBox Plane::UpdateAxisAlignedBoundingBox() const noexcept
	{
		AxisAlignedBoundingBox box = m_boundingBox;
		box.m_size = -1.f; // special case that is always hit to make infinite planes work easily
		return box;
	}
}


