#pragma once

#include "Vector3.h"

namespace BasicRenderer
{
	class PointLight
	{
	public:
		class Vector3 position;
		float intensity = 1.0f;

		PointLight() noexcept {}
		PointLight(float intensity_, const Vector3& pos) noexcept : position(pos), intensity(intensity_) {}
		PointLight(const PointLight& dl) noexcept : position(dl.position), intensity(dl.intensity) {}
		virtual ~PointLight() {}
	};
}