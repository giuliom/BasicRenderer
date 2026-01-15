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
		DirectionalLight(float intensity_, const Vector3& dir) : direction(dir), intensity(intensity_) {}
		DirectionalLight(const DirectionalLight& dl) : direction(dl.direction), intensity(dl.intensity) {}
		virtual ~DirectionalLight() {}

		void SetDirection(const Vector3& dir);
		inline Vector3 GetDirection() const { return direction; }
	};
}