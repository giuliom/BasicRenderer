#include "DirectionalLight.h"

namespace BasicRenderer
{
	void DirectionalLight::SetDirection(const Vector3& dir) noexcept
	{
		direction = dir.Normalize();
	}
}
