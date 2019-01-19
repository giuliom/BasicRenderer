#pragma once

#include "Vector3.h"
#include "Ray.h"

struct HitResult
{
	float t = 0;
	Vector3 pos;
	Vector3 normal;
};

class Hitable
{
public:
	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const = 0;
};