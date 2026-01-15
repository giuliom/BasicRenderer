#pragma once

#include "Vector3.h"

namespace BasicRenderer
{
	class PointLight
	{
	public:
		class Vector3 position;
		float intensity = 1.0f;

		PointLight() {}
		PointLight(float intensity_, const Vector3& pos) : position(pos), intensity(intensity_) {}
		PointLight(const PointLight& dl) : position(dl.position), intensity(dl.intensity) {}
		virtual ~PointLight() {}
	};
}