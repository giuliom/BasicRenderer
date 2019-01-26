#pragma once

#include "Vector3.h"

class PointLight
{
public:
	class Vector3 position;
	float intensity = 1.0f;

	PointLight() {}
	PointLight(float intensity_, const Vector3& pos) : intensity(intensity_), position(pos) {}
	PointLight(const PointLight& dl) : intensity(dl.intensity), position(dl.position) {}
	~PointLight() {}
};