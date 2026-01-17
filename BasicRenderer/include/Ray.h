#pragma once

#include"Vector3.h"

namespace BasicRenderer
{
	class Ray
	{
	public:
		Vector3 origin;
		Vector3 direction;
		Vector3 invDirection;

	public:
		Ray() {}
		Ray(const Vector3& ori, const Vector3& dir) : origin(ori), direction(dir.Normalize())
		{
			// Precompute inverse direction to avoid divisions in AABB tests
			invDirection = {
				1.0f / direction.x,
				1.0f / direction.y,
				1.0f / direction.z
			};
		}

		inline Vector3 GetPoint(float t) const { return origin + direction * t; }
	};
}
