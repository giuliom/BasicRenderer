#include "DirectionalLight.h"

void DirectionalLight::SetDirection(const Vector3 & dir)
{
	direction = dir.Normalize();
}
