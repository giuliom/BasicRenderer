#pragma once

#include "Vector3.h"

namespace BasicRenderer
{
	// Rays are always normalized at construction
	class Ray
	{
		Vector3 origin;
		Vector3 direction;
		Vector3 invDirection;

	public:
		Ray() {}
		Ray(const Vector3& ori, const Vector3& dir) : origin(ori), direction(dir.Normalize()), invDirection({
			1.0f / direction.x,
			1.0f / direction.y,
			1.0f / direction.z
		})
		{
		}

		Ray(const Ray& other) : origin(other.origin), direction(other.direction), invDirection(other.invDirection)
		{
		}

		Ray& operator=(const Ray& other)
		{
			origin = other.origin;
			direction = other.direction;
			invDirection = other.invDirection;
			return *this;	
		}

		inline const Vector3& GetOrigin() const { return origin; }
		inline const Vector3& GetDirection() const { return direction; }
		inline const Vector3& GetInvDirection() const { return invDirection; }
		inline const Vector3 GetPoint(float t) const { return origin + direction * t; }
	};
}
