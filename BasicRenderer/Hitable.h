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

class Hitable
{
protected:
	//TODO temporary implementation
	Material* material = nullptr;

public:

	Hitable();
	Hitable(Material* mat) : material(mat) {}
	Hitable(const Hitable& other) : material(other.GetMaterial()) {}

	virtual void ProcessForRendering() const = 0;
	virtual bool GetHit(const Ray& r, float tMin, float tMax, HitResult& result) const = 0;
	inline Material* GetMaterial() const { return material; }
};
