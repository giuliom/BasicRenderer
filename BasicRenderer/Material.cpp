#include "Material.h"
#include <random>
#include "PrimitiveTypes.h"

bool Material::Scatter(const Ray & rayIn, const HitResult & hit, Color& outColor, Color& directLight, Ray & scattered, const World& scene, Color(Material::*shading)(const World& w, const Vector3& pos, const Vector3& nrml)) const
{
	switch (type)
	{
	case Material::Type::METALLIC:
	{
		Vector3 reflected = Vector3::Reflect(rayIn.direction, hit.normal);
		scattered = Ray(hit.pos, reflected + UniforSampleInHemisphere(hit.normal) * (1.f - metallic));
		outColor = { 1.f, 1.f, 1.f }; //TODO placeholder
		return (Vector3::Dot(scattered.direction, hit.normal) > 0);
	}
		break;
	case Material::Type::DIELECTRIC:
	{
		Vector3 outNormal;
		Vector3 reflected = Vector3::Reflect(rayIn.direction, hit.normal);
		float ni_nt;
		outColor = { 1.f, 1.f, 1.f }; //TODO placeholder
		Vector3 refracted;
		float reflectionProb;
		float cos;
		if (Vector3::Dot(rayIn.direction, hit.normal) > 0.f)
		{
			outNormal = hit.normal * -1.f;
			ni_nt = refractiveIndex;
			cos = (Vector3::Dot(rayIn.direction, hit.normal) * refractiveIndex); // / rayIn.direction.Length(); == 1.f
		}
		else
		{
			outNormal = hit.normal;
			ni_nt = 1.f / refractiveIndex;
			cos = -(Vector3::Dot(rayIn.direction, hit.normal)); // / rayIn.direction.Length(); == 1.f
		}

		if (Refract(rayIn.direction, outNormal, ni_nt, refracted))
		{
			reflectionProb = Schlick(cos);
		}
		else
		{
			reflectionProb = 1.f;
		}

		if (UnitRandf() < reflectionProb)
		{
			scattered = Ray(hit.pos, reflected);
		}
		else
		{
			scattered = Ray(hit.pos, refracted);
		}

		return true;
	}
		break;
	default:
	{
		//TODO fix proper attenuation and color model to avoid canceling colors
		Vector3 target = hit.pos + UniforSampleInHemisphere(hit.normal);
		scattered = Ray(hit.pos, target - hit.pos);
		outColor = baseColor;
		directLight = (hit.material->*shading)(scene, hit.pos, hit.normal);
		return true;
	}
		break;
	}
}

bool Material::Refract(const Vector3 & v, const Vector3 & normal, float ni_nt, Vector3 & refracted) const
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

//TODO Use variadic functions for shading?
Color Material::NormalShading(const World & scene, const Vector3 & pos, const Vector3 & normal)
{
	return (normal + 1.0f) * 0.5f;
}

Color Material::LitShading(const World & scene, const Vector3 & pos, const Vector3 & normal)
{ //TODO check proper lighting formula
	return Vector3::One() *std::fmaxf(0.0f, Vector3::Dot(normal, scene.sun.GetDirection() * -1.f)) * scene.sun.intensity;
}

std::random_device rd;
std::default_random_engine engine(rd());
std::uniform_real_distribution<float> uniformDist(0, 1);

Vector3 UniforSampleInHemisphere(const Vector3& normal)
{
	Vector3 nt, nb;
	if (std::fabs(normal.x) > std::fabs(normal.y))
	{
		nt = Vector3(normal.z, 0.f, -normal.x) / sqrtf(normal.x * normal.x + normal.z * normal.z);
	}
	else
	{
		nt = Vector3(0.f, -normal.z, normal.y) / sqrtf(normal.y * normal.y + normal.z * normal.z);
	}
	nb = Vector3::CrossProduct(normal, nt);

	float r1 = uniformDist(engine);
	float r2 = uniformDist(engine);
	float sinTheta = sqrtf(1.f - r1 * r1);
	float phi = 2.f * PI * r2;
	float x = sinTheta * cosf(phi);
	float z = sinTheta * sinf(phi);
	Vector3 sample = Vector3(x, r1, z);
	
	return {
		sample.x * nb.x + sample.y * normal.x + sample.z * nt.x,
		sample.x * nb.y + sample.y * normal.y + sample.z * nt.y,
		sample.x * nb.z + sample.y * normal.z + sample.z * nt.z
	};
}
