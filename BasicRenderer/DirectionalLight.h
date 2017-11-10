#pragma once

#include "Vector3.h"

class DirectionalLight
{
public:

	Vector3 direction;
	float intensity = 1.0f;

	DirectionalLight() {}
	DirectionalLight(float intensity_, const Vector3& dir) : intensity(intensity_), direction(dir) {}
	DirectionalLight(const DirectionalLight& dl) : intensity(dl.intensity), direction(dl.direction) {}
	~DirectionalLight() {}

};