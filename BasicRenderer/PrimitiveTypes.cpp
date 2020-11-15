#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	AxisAlignedBoundingBox Sphere::UpdateAxisAlignedBoundingBox() const
	{
		return AxisAlignedBoundingBox(pos - Vector3(radius, radius, radius), pos + Vector3(radius, radius, radius));
	}

	AxisAlignedBoundingBox Plane::UpdateAxisAlignedBoundingBox() const
	{
		AxisAlignedBoundingBox box = m_boundingBox;
		box.m_size = -1.f; // special case that is always hit to make infinite planes work easily
		return box;
	}
}


