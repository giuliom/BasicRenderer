#pragma once

#include"Vector3.h"

class Ray 
{
public:
	Vector3 origin;
	Vector3 direction;

public:
	Ray() = delete;
	Ray(const Vector3& ori, const Vector3& dir) : origin(ori), direction(dir) {}

	inline Vector3 GetPoint(float t) const { return origin + direction * t; }

	//TODO move semantics
};