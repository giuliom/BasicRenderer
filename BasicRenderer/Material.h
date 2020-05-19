#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Primitive.h"
#include "World.h"

namespace BasicRenderer
{
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
		float emissive = 0.0f;
		float metallic = 1.0f;
		float roughness = 0.0f;
		float refractiveIndex = 1.5f;

		Material(const Color& base, Type type = Type::DIFFUSE) : baseColor(base), type(type) {}

		static inline bool Refract(const Vector3& v, const Vector3& normal, float ni_nt, Vector3& refracted)
		{
			Vector3 uv = v.Normalize();
			float dt = Vector3::Dot(uv, normal);
			float discriminant = 1.f - ni_nt * ni_nt * (1.f - dt * dt);
			if (discriminant > 0.f)
			{
				refracted = (uv - normal * dt) * ni_nt - normal * sqrtf(discriminant);
				return true;
			}
			return false;
		}

		static const Color MissingMaterialColor;

		Color NormalShading(const World& scene, const Vector3& pos, const Vector3& normal);
		Color LitShading(const World& scene, const Vector3& pos, const Vector3& normal);

		static inline float Schlick(float cos, float refractiveIndex)
		{
			float r0 = (1.f - refractiveIndex) / (1.f + refractiveIndex);
			r0 *= r0;
			return r0 + (1.f - r0) * powf(1.f - cos, 5.f);
		}
	};

	Vector3 UniforSampleInHemisphere(const Vector3& normal);

	//Returns a random float between 0 and 1
	inline float UnitRandf()
	{
		return ((float)rand() / (RAND_MAX));
	}
}