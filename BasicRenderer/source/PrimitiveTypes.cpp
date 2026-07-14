#include "PrimitiveTypes.h"
#include <algorithm>
#include <cmath>

namespace BasicRenderer
{
	AxisAlignedBoundingBox Sphere::UpdateAxisAlignedBoundingBox() const noexcept
	{
		return AxisAlignedBoundingBox(m_pos - Vector3(m_radius, m_radius, m_radius), m_pos + Vector3(m_radius, m_radius, m_radius));
	}

	AxisAlignedBoundingBox Plane::UpdateAxisAlignedBoundingBox() const noexcept
	{
		// Infinite planes are approximated with a MAX_EXTENT-sized finite box,
		// tight along the normal so axis-aligned planes stay flat
		const Vector3 extent(MAX_EXTENT * std::sqrt(std::max(0.f, 1.f - m_normal.x * m_normal.x)),
							 MAX_EXTENT * std::sqrt(std::max(0.f, 1.f - m_normal.y * m_normal.y)),
							 MAX_EXTENT * std::sqrt(std::max(0.f, 1.f - m_normal.z * m_normal.z)));
		return AxisAlignedBoundingBox(m_centre - extent, m_centre + extent);
	}
}


