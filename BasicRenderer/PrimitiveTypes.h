#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Vertex.h"

const float PI = 3.1415926f;


struct Sphere
{
public:
	Position pos;
	float radius;

	Sphere(Position pos_, float radius_) : pos(pos_), radius(radius_) {}
};


struct Cube
{
public:
	Position pos;
	float sideSize;

	Cube(Position pos_, float sideSize_) : pos(pos_), sideSize(sideSize_) {}
};
