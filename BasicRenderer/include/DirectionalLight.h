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

		DirectionalLight() noexcept {}
		DirectionalLight(float intensity_, const Vector3& dir) noexcept : direction(dir), intensity(intensity_) {}
		DirectionalLight(const DirectionalLight& dl) noexcept : direction(dl.direction), intensity(dl.intensity) {}
		virtual ~DirectionalLight() {}

		void SetDirection(const Vector3& dir) noexcept;
		inline Vector3 GetDirection() const noexcept { return direction; }
	};
}