#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	void Sphere::UpdateAxisAlignedBoundingBox()
	{
		m_boundingBox = AxisAlignedBoundingBox(pos - Vector3(radius, radius, radius), pos + Vector3(radius, radius, radius));
	}

	void Plane::UpdateAxisAlignedBoundingBox()
	{
		m_boundingBox.m_size = -1.f; // special case that is always hit to make infinite planes work easily
	}
}


