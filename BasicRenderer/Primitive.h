#pragma once

#include "Vector3.h"
#include <memory>
#include "Ray.h"

class Material;

struct HitResult
{
	float t = 0;
	Vector3 pos;
	Vector3 normal;
	Material* material = nullptr;
};

class Primitive
{
protected:
	//TODO temporary implementation
	Material* material = nullptr;

public:

	Primitive();
	Primitive(Material* mat) : material(mat) {}
	Primitive(const Primitive& other) : material(other.GetMaterial()) {}

	virtual void ProcessForRendering() const = 0;
	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const = 0;
	inline Material* GetMaterial() const { return material; }
};
