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
		Ray() noexcept {}
		Ray(const Vector3& ori, const Vector3& dir) noexcept : origin(ori), direction(dir.Normalize()), invDirection({
			1.0f / direction.x,
			1.0f / direction.y,
			1.0f / direction.z
		})
		{
		}

		Ray(const Ray& other) noexcept : origin(other.origin), direction(other.direction), invDirection(other.invDirection)
		{
		}

		Ray& operator=(const Ray& other) noexcept
		{
			origin = other.origin;
			direction = other.direction;
			invDirection = other.invDirection;
			return *this;	
		}

		inline const Vector3& GetOrigin() const noexcept { return origin; }
		inline const Vector3& GetDirection() const noexcept { return direction; }
		inline const Vector3& GetInvDirection() const noexcept { return invDirection; }
		inline const Vector3 GetPoint(float t) const noexcept { return origin + direction * t; }
	};
}
