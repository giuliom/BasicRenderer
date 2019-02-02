#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Vertex.h"
#include "Ray.h"
#include "Hitable.h"

const float PI = 3.1415926f;

class Material;

//TODO World space assumed for all these primitives

struct Sphere : public Hitable
{
public:
	Vector3 pos;
	float radius;

	Sphere() {}
	Sphere(Vector3 pos_, float radius_) : pos(pos_), radius(radius_) {}
	Sphere(Vector3 pos_, float radius_, Material* mat) : Hitable(mat), pos(pos_), radius(radius_) {}

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;
};

class Plane : public Hitable
{
	Vector3 centre, normal;

public:
	Plane() = delete;
	Plane(const Vector3& centre, const Vector3& normal, Material* mat) : Hitable(mat), centre(centre), normal(normal.Normalize()) {}

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override;

};


struct Cube : public Hitable
{
public:
	Vector3 pos;
	float sideSize;

	Cube(Vector3 pos_, float sideSize_) : pos(pos_), sideSize(sideSize_) {}

	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const override { return false; }
};
