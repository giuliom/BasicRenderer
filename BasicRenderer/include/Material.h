#pragma once

#include <cmath>
#include "Ray.h"
#include "Vector3.h"
#include "Primitive.h"
#include "World.h"
#include "RenderState.h"
#include "Color.h"

namespace BasicRenderer
{
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

		Material(const Color& base, Type type = Type::DIFFUSE) : type(type), baseColor(base) {}

		// Ray directions are always normalized
		static inline bool Refract(const Ray& r, const Vector3& normal, float ni_nt, Vector3& refracted)
		{
			float dt = Vector3::Dot(r.GetDirection(), normal);
			float discriminant = 1.f - ni_nt * ni_nt * (1.f - dt * dt);
			if (discriminant > 0.f)
			{
				refracted = (r.GetDirection() - normal * dt) * ni_nt - normal * std::sqrt(discriminant);
				return true;
			}
			return false;
		}

		static const Color MissingMaterialColor;

		Color NormalShading(const RenderState& state, const Vector3& pos, const Vector3& normal) const;
		Color LitShading(const RenderState& state, const Vector3& pos, const Vector3& normal) const;

		static inline float Schlick(float cos, float refractiveIndex)
		{
			float r0 = (1.f - refractiveIndex) / (1.f + refractiveIndex);
			r0 *= r0;
			const float x = 1.f - cos;
			const float x2 = x * x;
			const float x5 = x2 * x2 * x;
			return r0 + (1.f - r0) * x5;
		}
	};

	Vector3 UniformSampleInHemisphere(const Vector3& normal);
}