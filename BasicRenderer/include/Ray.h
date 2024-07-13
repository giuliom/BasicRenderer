#pragma once

#include"Vector3.h"

namespace BasicRenderer
{
	class Ray
	{
	public:
		Vector3 origin;
		Vector3 direction;

	public:
		Ray() {}
		Ray(const Vector3& ori, const Vector3& dir) : origin(ori), direction(dir.Normalize()) {}

		inline Vector3 GetPoint(float t) const { return origin + direction * t; }
	};
}
