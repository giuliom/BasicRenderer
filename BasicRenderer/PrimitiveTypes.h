#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Vertex.h"
#include "Ray.h"

const float PI = 3.1415926f;


struct Sphere
{
public:
	Vector3 pos;
	float radius;

	Sphere() {}
	Sphere(Vector3 pos_, float radius_) : pos(pos_), radius(radius_) {}

	float GetHit(const Ray& r);
};


struct Cube
{
public:
	Vector3 pos;
	float sideSize;

	Cube(Vector3 pos_, float sideSize_) : pos(pos_), sideSize(sideSize_) {}
};
