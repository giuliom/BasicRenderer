#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Hitable.h"
#include "World.h"

using Color = Vector3;

class Material
{
public:
	Color diffuseColor;
	float metallic = 0.f;

	Material(const Color& diffuse) : diffuseColor(diffuse) {}

	virtual bool Scatter(const Ray& rayIn, const HitResult& hit, Vector3& attenuation, Ray& scattered, const World& scene, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml)) const;

	Color NormalShading(const World& scene, const Vector3& pos, const Vector3& normal);
	Color LitShading(const World& scene, const Vector3& pos, const Vector3& normal);
};

Vector3 RandomPointInUnitSphere();