#pragma once

#include "Vector3.h"

namespace BasicRenderer
{
	class DirectionalLight
	{
	protected:
		class Vector3 direction;

	public:
		float intensity = 1.0f;

		DirectionalLight() {}
		DirectionalLight(float intensity_, const Vector3& dir) : intensity(intensity_), direction(dir) {}
		DirectionalLight(const DirectionalLight& dl) : intensity(dl.intensity), direction(dl.direction) {}
		~DirectionalLight() {}

		void SetDirection(const Vector3& dir);
		inline Vector3 GetDirection() const { return direction; }
	};
}