#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Hitable.h"
#include "World.h"

using Color = Vector3;

class Material
{
public:

	enum class Type
	{
		DIFFUSE,
		METALLIC,
		DIELECTRIC
	};

	Type type;

	Color baseColor;
	float metallic = 1.0f;
	float roughness = 0.0f;
	float refractiveIndex = 1.5f;

	Material(const Color& base, Type type = Type::DIFFUSE) : baseColor(base), type(type) {}

	virtual bool Scatter(const Ray& rayIn, const HitResult& hit, Vector3& attenuation, Ray& scattered, const World& scene, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml)) const;

	bool Refract(const Vector3& v, const Vector3& normal, float ni_nt, Vector3& refracted) const;

	Color NormalShading(const World& scene, const Vector3& pos, const Vector3& normal);
	Color LitShading(const World& scene, const Vector3& pos, const Vector3& normal);

	inline float Schlick(float cos) const
	{
		float r0 = (1.f - refractiveIndex) / (1.f + refractiveIndex);
		r0 *= r0;
		return r0 + (1.f - r0) * powf(1.f - cos, 5.f);
	}
};

Vector3 RandomPointInUnitSphere();

//Returns a random float between 0 and 1
inline float UnitRandf()
{
	return ((float)rand() / (RAND_MAX));
}