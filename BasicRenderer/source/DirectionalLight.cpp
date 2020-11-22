#include "DirectionalLight.h"

namespace BasicRenderer
{
	void DirectionalLight::SetDirection(const Vector3& dir)
	{
		direction = dir.Normalize();
	}
}
